#include "Server.hpp"
#include "Parser.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>

bool Server::Init()
{
    serverFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int opt = 1;
    setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddress;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(6667);
    serverAddress.sin_family = AF_INET;

    int res = bind(serverFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (res == -1)
    {
        perror("BIND Error");
        return false;
    }

    listen(serverFD, 1024);

    fds.push_back((pollfd){serverFD, POLLIN, 0});

    isRunning = true;

    return true;
}

void Server::Run()
{
    while (isRunning)
    {
        poll(fds.data(), fds.size(), -1);

        // Servidor recebeu algo!
        if (fds.at(0).revents & POLLIN)
        {
            int connectionFD = accept(serverFD, NULL, NULL);
            if (connectionFD >= 0)
            {
                fds.push_back((pollfd){connectionFD, POLLIN, 0});
                // c.SetHostname("42.pt");
                clients[connectionFD] = Client();
                clients[connectionFD].SetFD(connectionFD);
                clients[connectionFD].SetHostname("42.pt");
            }
        }

        for (size_t i = 1; i < fds.size();)
        {
            if (fds.at(i).revents & POLLIN)
            {
                char incoming[513];
                int numBytes = recv(fds.at(i).fd, incoming, 512, 0);

                if (numBytes <= 0)
                {
                    close(fds.at(i).fd);
                    fds.erase(fds.begin() + 1);
                    continue;
                }
                else
                {
                    Parser parser;
                    std::vector<std::string> messages = parser.Split(std::string(incoming, numBytes));

                    Client &c = clients[fds.at(i).fd];

                    for (size_t j = 0; j < messages.size(); ++j)
                    {
                        std::string message = messages.at(j);
                        CommandType cmd = parser.ProcessLine(message);

                        std::cout << "Client says: " << message << "\n";

                        // std::cout << "switch: " << (int)parser.commandType << "\n";
                        switch (cmd)
                        {
                        case CAP:
                            HandleCAP(&c, message);
                            break;
                        case NICK:
                            std::cout << "NICK " << parser.commandNickData.nickname << "\n";
                            c.SetNickname(parser.commandNickData.nickname);
                            break;
                        case USER:
                            std::cout << "NICK " << parser.commandUserData.username << "\n";
                            c.SetUsername(parser.commandUserData.username);
                            break;
                        case PRIVMSG:
                            break;
                        case JOIN:
                            HandleJoin(&c, parser.commandJoinData);
                            break;
                        case ERROR:
                            break;
                        default:
                            std::cout << "UNKNOWN";
                            break;
                        }
                    }
                }
                // incoming[0] = 0;
            }
            i++;
        }
    }
}

void Server::HandleCAP(Client *c, std::string line)
{
    (void)c;
    if (line.compare(0, 6, "CAP LS") == 0)
    {
        std::string response = ":42.pt CAP * LS 302 :\r\n";
        SendToClient(c, response);
    }
    else if (line.compare(0, 7, "CAP END") == 0)
    {
        std::string response = "42.pt 001 " + c->GetNickname() + " :";
        response += "Hello dear evaluator!\r\n";
        SendToClient(c, response);
    }
}

void Server::HandleJoin(Client *c, CommandJoinData data)
{
    std::string res = GetPrefix(c) + " JOIN " + data.channelName + "\r\n";
    SendToClient(c, res);
}

std::string Server::GetPrefix(Client *c)
{
    //:<nickname>!<username>@<hostname>
    std::string prefix;
    prefix = ":" + c->GetNickname() + "!" + c->GetUserName() + "@" + c->GetHostName();
    return prefix;
}

void Server::SendToClient(Client *c, std::string message)
{
    std::cout << "Server says: " + message << "\n";
    send(c->GetFD(), message.c_str(), message.size(), 0);
}