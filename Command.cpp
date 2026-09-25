
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

        if (strings.size() == 3)
        {
            std::cout << strings[1] << std::endl;
            std::cout << strings[2] << std::endl;
        }

        return NICK;
    }

    /*
    * PRIVMSG #general :Hello everyone!
    */
    else if (data.compare(0, 7, "PRIVMSG") == 0)
    {

        std::vector<std::string> strings = getStrings(data, ":");

        if (strings.size() > 2)
            msg.data = strings[0];

        return MSG;
    }

    /*
    * JOIN #general
    */
    else if (data.compare(0, 4, "JOIN") == 0)
    {
        return JOIN;
    }
    

    return ANOTHER;

}
