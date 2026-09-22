#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>


#include "Client.hpp"

class Server
{
public:
    Server();

private:
    std::vector<Client> clients;        // Todos os clientes connectados ao servidor
};

#endif