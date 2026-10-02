#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
#include "Client.hpp"
#include "Parser.hpp"

#include <map>
#include <vector>
#include <poll.h>

class Server
{
private:
    std::vector<pollfd> fds;
    std::map<std::string, Channel*> channels;
    std::map<int, Client> clients;
    int serverFD;
    bool isRunning;

    // Handlers
    void HandleCAP(Client* c, std::string line);
    void HandleJoin(Client *c, CommandJoinData data);
    void HandleNICK(Client *c, CommandNickData data);
    //void HandleJOIN();
    //void HandlePRIVMSG();
    //void HandleUSER();

    // Send
    void SendToClient(Client *c, std::string message);

public:
    void Run();
    bool Init();
    
};

#endif