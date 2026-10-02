#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

struct CommandUserData
{
    std::string username;
};

struct CommandNickData
{
    std::string nickname;
};

struct CommandJoinData
{
    std::string channel;
};

struct CommandPrivData
{
    std::string target;
    std::string content;
    bool isChannel;
};

enum CommandType
{
    ERROR,
    CAP,
    NICK,
    USER,
    PRIVMSG,
    JOIN,
    QUIT
};

class Parser
{
public:
    std::vector<std::string> Split(std::string mesessages);
    std::vector<std::string> Split(std::string lines, std::string split);

    CommandType ProcessLine(std::string message);

    CommandUserData commandUserData;
    CommandNickData commandNickData;
    CommandJoinData commandJoinData;
    CommandPrivData commandPrivData;
};

#endif