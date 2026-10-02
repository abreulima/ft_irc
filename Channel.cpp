#include "Channel.hpp"

void Channel::Add(Client *c, Role r)
{
    clients[c] = r;
}