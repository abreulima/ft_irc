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
    std::string channelName;
};

struct CommandPrivData
{
    std::string channelOrUser;
    std::string content;
};

enum CommandType
{
    ERROR,
    CAP,
    NICK,
    USER,
    PRIVMSG,
    JOIN,
};

class Parser
{
public:
    std::vector<std::string> Split(std::string mesessages);
    std::vector<std::string> Split(std::string lines, std::string split);

    CommandType ProcessLine(std::string message);

    //
    CommandUserData commandUserData;
    CommandNickData commandNickData;
    CommandJoinData commandJoinData;
    CommandPrivData commandPrivData;
};

#endif