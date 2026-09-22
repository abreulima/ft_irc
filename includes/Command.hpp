#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>

struct Join
{
    std::string channel;
};

struct Nick
{
    std::string nick;
};

enum class CommandType
{
    JOIN,
    NICK,
    ERROR
};

class Command
{
    public:
        CommandType Command(std::string message);
        Nick nick;
        Join join;

        // Exemplo
        CommandType Process()
        {

            // se menssagem comeca com NICK
            {
                nick.nick = "Ivan";
                return CommandType::NICK;
            }

            // se menssagem comeca com JOIN
            {
                join.channel = "pastel";
                return CommandType::JOIN;
            }
            
        }
        
        
};

#endif COMMAND_HPP
