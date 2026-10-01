#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"

#include <string>
#include <map>

struct Role
{
    bool isOperator;
};

class Channel
{
private:
    std::string _name;
    std::map<Client*, Role> clients;
public:
    
};

#endif