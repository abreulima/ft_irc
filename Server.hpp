#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"

#include <vector>
#include <map>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <vector>
#include <cstdlib>
#include <poll.h>

#define BS 512
#define USER_MAX 512

typedef struct pollfd pollfd_t;
typedef struct sockaddr_in sockaddr_in_t;

class Server
{
private:
    char buf[BS + 1];
    std::vector<pollfd_t> fds;
    bool isRunning;
    int serv_fd;
    void polling();
    //std::vector<Client> clients;
    std::map<pollfd_t, Client> clients;


public:
    Server();
    ~Server();
    bool Init();
    void Run();
};

#endif