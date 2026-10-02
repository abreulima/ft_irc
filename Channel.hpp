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
public:
    void Add(Client *c, Role r);
    std::map<Client*, Role> clients;

};

#endif