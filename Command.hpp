#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <iostream>

struct NickUser
{
    std::string nick;
    std::string name;
};

struct NICK
{

};

enum CommandType
{
    NICK,
    MSG,
    JOIN,
    ANOTHER

    //

};

struct MSG 
{
    std::string data;
};

class Command
{
private:
    //std::string toSend;
    struct MSG msg;

public:
    CommandType Parse(std::string data);
};

#endif