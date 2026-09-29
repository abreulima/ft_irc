#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>

class Client;

struct MemberChannel
{
    bool isOperator;
    bool isBanned;
    bool isRegistered;
    std::string password;
};

class Channel
{
public:
    std::string name;
    std::map<Client*, MemberChannel> channels;

    bool IsMember(Client *c)
    {
        if (channels.count(c) > 0)
            return true;
        return false;
    }

    Client* GetorAdd(std::string channel, std::string)
    {

        if (IsMember(c))
            return c;

        MemberChannel memberChannel;
        channels[c] = memberChannel;

        return ch
    };

};

#endif