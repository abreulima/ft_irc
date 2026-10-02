#include "Channel.hpp"

void Channel::Add(Client *c, Role r)
{
    clients[c] = r;
}

void Channel::Remove(Client *c)
{
    clients.erase(c);
}

size_t Channel::CountMembers()
{
    return clients.size();
}

bool Channel::IsMember(Client *c)
{
    if (clients.find(c) != clients.end())
        return true;
    return false;
}

