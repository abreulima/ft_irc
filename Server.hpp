#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp";

#include <map>

class Server
{
private:
    std::map<int, fd> fds;
    std::map<std::string, Channel*> channels;
public:
    void Run();
    
};

#endif