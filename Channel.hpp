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

    Channel(const std::string &name) : name(name) {}

    bool HasMember(Client *c)
    {
        return (channels.count(c) > 0);
    }

    Client* AddMember(Client* c)
    {
        MemberChannel memberChannel;
        channels[c] = memberChannel;
        return c;
    };

};

#endif