
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
    Raw data
    */
    std::cout << data << "\n";

    // CAP LS 302
    if (data.compare(0, 3, "CAP") == 0)
    {
        return CAP;
    }

    // NICK leschunc
    else if (data.compare(0, 4, "NICK") == 0)
    {
        std::vector<std::string> strings = getStrings(data, " ");

        if (strings.size() > 1)
            nickData.nick = strings.at(1);

        return NICK;
    }

    // USER leschunc 0 * :realname
    else if (data.compare(0, 4, "USER") == 0)
    {
        std::vector<std::string> strings = getStrings(data, " ");

        if (strings.size() > 1)
            userData.name = strings.at(1);

        return USER;
    }

    // PRIVMSG #general :Hello everyone!
    else if (data.compare(0, 7, "PRIVMSG") == 0)
    {
        std::vector<std::string> strings = getStrings(data, " ");

        if (strings.size() > 1)
            msgData.channelOrUser = strings[1];

        size_t colon = data.find(":");

        if (colon != std::string::npos)
            msgData.message = data.substr(colon + 1);

        return MSG;
    }

    // JOIN #general
    else if (data.compare(0, 4, "JOIN") == 0)
    {
        std::vector<std::string> strings = getStrings(data, " ");

        if (strings.size() > 1)
            joinData.channel = strings.at(1);

        return JOIN;
    }

    return ANOTHER;
}
