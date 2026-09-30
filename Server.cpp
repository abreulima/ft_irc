#include "Server.hpp"
#include "Command.hpp"
#include "Connection.hpp"

#include <cerrno>
#include <cstdio>

Server::Server() : isRunning(true)
{
    std::memset(buf, 0, BS + 1);
}

Server::~Server()
{
}

bool Server::Init()
{
    serv_fd = socket(AF_INET, SOCK_STREAM, 0);

    // resolve o problema do endereco em uso
    int opt = 1;
    setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    sockaddr_in_t serv_addr;

    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(6667);
    serv_addr.sin_family = AF_INET;

    if (bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("DEU RUIM: ");
        return false;
    }

    listen(serv_fd, USER_MAX);

    fds.push_back((pollfd_t){serv_fd, POLLIN, 0});
    return true;
}

void Server::Run()
{
    int new_cli;

    while (isRunning)
    {
        poll(fds.data(), fds.size(), -1);

        if (fds.at(0).revents & POLLIN)
        {
            new_cli = accept(serv_fd, NULL, NULL);

            // remember this usermax
            if (new_cli >= 0)
            {
                fds.push_back((pollfd_t){new_cli, POLLIN, 0});
                clients[new_cli] = Client();
                // buffers[new_cli] = "";
            }
        }

        for (size_t i = 1; i < fds.size();)
        {
            if (fds.at(i).revents & POLLIN)
            {
                // magic number review
                int read_bytes = recv(fds.at(i).fd, buf, BS, 0);

                if (read_bytes <= 0)
                {
                    close(fds.at(i).fd);
                    fds.erase(fds.begin() + i);
                    continue;
                }

                else
                {
                    buf[read_bytes] = 0;

                    // transforma num std::string para trabalhar melhor
                    std::string incoming(buf);

                    size_t start = 0;
                    size_t end;

                    while ((end = incoming.find("\r\n", start)) != std::string::npos)
                    {

                        std::string line = incoming.substr(start, end - start);

                        Command cmd;
                        CommandType type = cmd.Parse(line);

                        // Adiciona
                        if (type == CAP)
                        {
                            if (line.compare(0, 6, "CAP LS") == 0)
                            {
                                std::string message = ":42.pt CAP * LS :\r\n";
                                send(fds.at(i).fd, message.c_str(), message.size(), 0);
                            }
                            else if (line == "CAP END")
                            {
                                Client &c = clients[fds.at(i).fd];

                                std::string message =
                                    ":42.pt 001 " + c.GetNick() +
                                    " :Welcome to the 42.pt IRC Network\r\n";

                                send(fds.at(i).fd, message.c_str(), message.size(), 0);
                            }
                        }

                        else if (type == JOIN)
                        {
                            // Pega o Client de quem enviou
                            Client &c = clients[fds.at(i).fd];

                            std::string channelName = cmd.joinData.channel;

                            std::map<std::string, Channel>::iterator it;
                            it = channels.find(channelName);

                            // canal nao existe
                            if (it == channels.end())
                            {
                                channels.insert(std::make_pair(channelName, Channel(channelName)));
                                it = channels.find(channelName);
                            }

                            Channel &channel = it->second;

                            // usuario nao existe no canal
                            if (!channel.HasMember(&c))
                            {
                                channel.AddMember(&c);
                            }

                            // CONFIRMA O JOIN
                            //: Nick!username@localhost JOIN #canal
                            std::string msg =
                                ":" + c.GetNick() + "!" + c.GetName() +
                                "@42.pt JOIN " + cmd.joinData.channel + "\r\n";

                            send(fds.at(i).fd, msg.c_str(), msg.size(), 0);

                            // NAMES
                            std::string names =
                                ":42.pt 353 " + c.GetNick() +
                                " = " + channelName +
                                " :@" + c.GetNick() + "\r\n";

                            send(fds.at(i).fd, names.c_str(), names.size(), 0);

                            // End of NAMES
                            std::string endNames =
                                ":42.pt 366 " + c.GetNick() +
                                " " + channelName +
                                " :End of /NAMES list.\r\n";

                            send(fds.at(i).fd, endNames.c_str(), endNames.size(), 0);

                            // Debug
                            std::cout
                                << "Cliente "
                                << c.GetName()
                                << "entrou no canal "
                                << cmd.joinData.channel
                                << std::endl;
                        }

                        else if (type == MSG)
                        {

                            // Pega o Client de quem enviou
                            Client &c = clients[fds.at(i).fd];

                            // :Name!usernamek@localhost PRIVMSG #canal> :Mensagem
                            std::string message = ":" + c.GetName() + "!" + c.GetNick() + "@42.pt " + "PRIVMSG " + cmd.msgData.channelOrUser + " :" + cmd.msgData.message + "\r\n";

                            // Envia a mensagem para todos os outros clientes (aka fds)
                            for (size_t j = 1; j < fds.size(); j++)
                            {
                                if (i != j)
                                {
                                    std::string msg = message;
                                    send(fds.at(j).fd, msg.c_str(), msg.size(), 0);
                                    std::cout << " " << message << std::endl;
                                }
                            }

                            // DEBUG
                            std::cout
                                << "Cliente "
                                << c.GetName()
                                << "enviou no canal"
                                << "#general" // ainda nao tem parsing do canal
                                << "a mensagem: "
                                << cmd.msgData.message
                                << std::endl;
                        }

                        else if (type == NICK)
                        {
                            Client &c = clients[fds.at(i).fd];
                            c.SetNick(cmd.nickData.nick);
                        }

                        else if (type == USER)
                        {
                            Client &c = clients[fds.at(i).fd];
                            c.SetName(cmd.userData.name);
                        }

                        else if (type == MODE)
                        {

                        }

                        else if (type == WHO)
                        {
                            
                        }

                        else
                        {
                            std::cout
                                << "Comando desconhecido: "
                                << line
                                << std::endl;
                        }

                        start = end + 2;
                    }
                }
            }
            ++i;
        }
    }
}