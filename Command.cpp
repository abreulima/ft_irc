
#include <vector>

#include "Command.hpp"

std::vector<std::string> getStrings(std::string s, std::string split)
{
    std::vector<std::string> res;

    size_t start = 0;
    size_t pos;

    while ((pos = s.find(split, start)) != std::string::npos)
    {
        res.push_back(s.substr(start, pos - start));
        start = pos + split.size();
    }

    res.push_back(s.substr(start));
    return res;
}

CommandType Command::Parse(std::string data)
{

    /*
    CAP LS 302
    NICK leschunc
    USER leschunc 0 * :realname
    */
    if (data.compare(0, 6, "CAP LS") == 0)
    {
        std::vector<std::string> strings = getStrings(data, "\r\n");
        if (strings.size() > 2)
        {
            std::vector<std::string> substringNick = getStrings(strings[1], " "); //    NICK leschunc
            nickData.nick = substringNick.at(1);

            std::vector<std::string> substringRealname = getStrings(strings[2], ":"); //    USER leschunc 0 * :realname
            nickData.name = substringRealname.at(1);
        }
        return NICK;
    }

    /*
    * PRIVMSG #general :Hello everyone!
    */
    else if (data.compare(0, 7, "PRIVMSG") == 0)
    {
        std::vector<std::string> strings = getStrings(data, ":");
        if (strings.size() > 1)
            msgData.message = strings[1];
        return MSG;
    }

    /*
    * JOIN #general
    */
    else if (data.compare(0, 4, "JOIN") == 0)
    {
        std::vector<std::string> strings = getStrings(data, " ");
        if (strings.size() > 1)
            joinData.channel = strings.at(1); // #general
        return JOIN;
    }
    
    return ANOTHER;
}
