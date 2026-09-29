
#include "Connection.hpp"
#include <stdexcept>

Connection::Connection(std::string username, std::string nick) : name(username), nick(nick)
{
    
}


Connection::Connection()
{

}

Connection::~Connection()
{

}

std::string Connection::GetName()
{
    return name;
}

std::string Connection::GetNick()
{
    return nick;
}
