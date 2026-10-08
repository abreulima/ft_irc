#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

namespace Command
{

    struct User
    {
        std::string username;
    };

    struct Nick
    {
        std::string nickname;
    };

    struct Join
    {
        std::string channel;
    };

    struct Kick
    {
        std::string channel;
        std::string nickname;
    };

    struct Priv
    {
        std::string target;
        std::string content;
        bool isChannel;
    };

    struct Who
    {
        std::string channel;
    };

    struct Mode
    {
        std::string channel;
    };

}

enum Type
{
    ERROR,
    CAP,
    NICK,
    USER,
    PRIVMSG,
    JOIN,
    KICK,
    MODE,
    WHO,
    QUIT
};

class Parser
{
public:
    std::vector<std::string> Split(std::string mesessages);
    std::vector<std::string> Split(std::string lines, std::string split);

    Type ProcessLine(std::string message);
    Command::User user;
    Command::Nick nick;
    Command::Kick kick;
    Command::Join join;
    Command::Priv priv;
    Command::Who who;
    Command::Mode mode;
};

#endif
