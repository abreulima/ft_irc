#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <iostream>


enum CommandType
{
    NICK,
    MSG,
    JOIN,
    ANOTHER

    //

};

struct nick_t
{
    std::string nick;
    std::string name;
};

struct msg_t 
{
    std::string message;
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
    struct nick_t nickData;
    struct join_t joinData;
};

#endif