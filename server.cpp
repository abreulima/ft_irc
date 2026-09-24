#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <stdbool.h>
#include <vector>
#include <cstdlib>

#define BS 512
#define USER_MAX 512

typedef struct pollfd pollfd_t;
typedef struct sockaddr_in sockaddr_in_t;

int main()
{
    std::vector<pollfd_t> fds;

    char buf[BS + 1] = {0};

    int serv_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in_t serv_addr;

    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(6667);
    serv_addr.sin_family = AF_INET;

    if (bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        {std::cout << "deu ruim\n"; std::exit(1);}

    listen(serv_fd, USER_MAX);

    fds.push_back((pollfd_t){serv_fd, POLLIN, 0});

    while (true)
    {
        poll(fds.data(), fds.size(), -1);

        if (fds.at(0).revents & POLLIN)
        {
            int new_cli = accept(serv_fd, NULL, NULL);

            // remember this usermax
            if (new_cli >= 0)
            {
                fds.push_back((pollfd_t){new_cli, POLLIN, 0});

                send(new_cli, ":irc.rede-exemplo.pt 001 mama :Oi mamaco!\r\n", 44, 0);
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
                    std::cout << "User: " << fds.at(i).fd << buf << "\n";
                    std::string msg = ":leschunc!leschunc@seu-host JOIN #canal\r\n";
                    send(fds.at(i).fd, msg.data(), msg.size(), 0);

                    for (size_t j = 0; j < fds.size(); j++)
                    {
                        if (j != i)
                        {
                            // msg = fds.
                            msg = ":Nick!User@Host PRIVMSG #canal :Texto da mensagem\r\n";
                            std::cout << msg;
                            // send(fds.at(j).fd, msg.data(), msg.size(), 0);
                        }
                    }
                }
            }
            ++i;
        }
    }

    return 0;
}
