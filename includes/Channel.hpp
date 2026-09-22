#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>

#include "Client.hpp"

class Channel 
{
    std::vector<Client*> clients;           // Endereco dos clientes conectados
};

#endif