#include "Server.hpp"
#include "Command.hpp"

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
                
                
                
                std::string message(":42.pt 001 leschunc :Oi lindo!\r\n");
                send(new_cli, message.c_str(), message.size(), 0);
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
                    //std::cout << "User: " << fds.at(i).fd << buf << "\n";

                    std::string incoming(buf);

                    Command cmd;
                    CommandType type =  cmd.Parse(incoming);

                    // Adiciona
                    if (type == NICK)
                    {
                        std::cout << cmd.nickData.name << std::endl; 
                        Client c(cmd.nickData.name, cmd.nickData.nick);
                        clients[fds.at(i).fd] = c;
                        std::cout << "Cliente conectado: " << clients[fds.at(i).fd].GetNick() << " " << clients[fds.at(i).fd].GetName() << std::endl;
                    }
                    
                    else if (type == JOIN)
                    {
                        // Pega o Client de quem enviou
                        Client c = clients[fds.at(i).fd];

                        //:Alice!alice@localhost JOIN #general
                        // Nick username host
                        std::string msg = ":" + c.GetNick() + "!" + c.GetName() + "@42.pt JOIN " + cmd.joinData.channel;
                        //std::string msg = ":leschunc!leschunc@42.pt JOIN #general\r\n";
                        send(fds.at(i).fd, msg.c_str(), msg.size(), 0);

                    }

                    else if (type == MSG)
                    {

                        // Pega o Client de quem enviou
                        Client c = clients[fds.at(i).fd];

                        // Monta a menssagem a ser enviada
                        // Exemplo
                        // :Name!nick@42.pt PRIVMSG #general :<Message>
                        std::string message = ":" + c.GetName() + "!" + c.GetNick() + "@42.pt " + "PRIVMSG #general :" + cmd.msgData.message;

                        // Envia a vamos enviar para todos
                        for (size_t j = 1; j < fds.size(); j++)
                        {
                            if (i != j)
                            {
                                std::string msg = message;
                                send(fds.at(j).fd, msg.c_str(), msg.size(), 0);
                            }
                        }

                    }

               
                }
            }
            ++i;
        }
    }
}