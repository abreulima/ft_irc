
#include "Client.hpp"
#include <stdexcept>

Client::Client(std::string username, std::string nick) : name(username), nick(nick)
{
    
}


Client::Client()
{

}

Client::~Client()
{

}

std::string Client::GetName()
{
    return name;
}

std::string Client::GetNick()
{
    return nick;
}
