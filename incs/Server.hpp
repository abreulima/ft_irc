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
        std::map<std::string, Channel> channels;
        std::map<int, Client> clients;
        int serverFD;
        bool isRunning;

        // Handlers
        void HandleCAP(Client* c, std::string line);
        void HandleJoin(Client *c, Command::Join data);
        void HandleKick(Client *c, Command::Kick data);
        void HandleNick(Client *c, Command::Nick data);
        void HandlePrivMsg(Client *c, Command::Priv data);
        void HandleWho(Client *c, Command::Who data);
        void HandleMode(Client *c, Command::Mode data);
        void HandleQuit(Client *c);

        //void HandlePRIVMSG();
        //void HandleUSER();

        // Send
        void SendToClient(Client c, std::string message);
        void SendToChannel(Channel c, std::string message, Client exclude);

    public:
        void Run();
        bool Init();
    
};

#endif
