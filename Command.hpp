#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <iostream>


enum CommandType
{
    NICK,
    USER,
    MSG,
    JOIN,
    ANOTHER,
    ERROR,
    WHO,
    MODE,
    CAP
};

struct nick_t
{
    std::string nick;
};

struct cap
{
    std::string nick;
    std::string name;
    std::string password;
};

struct msg_t 
{
    std::string message;
    std::string channelOrUser;
};

struct user_t
{
    std::string name;
};

struct join_t
{
    std::string channel;
};

class Command
{
private:
    std::string toSend;

public:
    CommandType Parse(std::string data);
    struct msg_t msgData;
    struct cap capData;
    struct join_t joinData;
    struct nick_t nickData;
    struct user_t userData;
};

#endif