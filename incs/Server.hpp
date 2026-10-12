#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
#include "Client.hpp"
#include "Parser.hpp"

#include <map>
#include <vector>
#include <poll.h>

#define RPL_TOPIC 			std::string("332")
#define RPL_CHANNELMODEIS 	std::string("324")
#define ERR_NICKNAMEINUSE 	std::string("433")
#define ERR_NEEDMOREPARAMS 	std::string("461")
#define ERR_NOSUCHCHANNEL 	std::string("403")

class Server
{
private:
	std::vector<pollfd>				fds;
	std::map<std::string, Channel>	channels;
	std::map<int, Client>			clients;
	int 							serverFD;
	int								port;
	std::string						password;
	bool 							isRunning;

	// Handlers
	void handleCommands(size_t fd_id, std::vector<std::string>& messages);

	void HandleCAP(Client *c, std::string line);
	void HandleInvite(Client *c, Command::Invite data);
	void HandleJoin(Client *c, Command::Join data);
	void HandleKick(Client *c, Command::Kick data);
	void HandleTopic(Client *c, Command::Topic data);
	void HandleNick(Client *c, Command::Nick data);
	void HandlePrivMsg(Client *c, Command::Priv data);
	void HandleWho(Client *c, Command::Who data);
	void HandleMode(Client *c, Command::Mode data);
	void HandleQuit(Client *c, Command::Quit data);

	// void HandlePRIVMSG();
	// void HandleUSER();

	// Send
	void SendToClient(Client c, std::string message);
	void SendToChannel(Channel c, std::string message, Client *exclude);
	void SendToChannels(std::vector<Channel *> channels, std::string message, Client *exclude);

	// Channel Manager
	std::vector<Channel*> GetChannelsByClient(Client *c);
	Channel* GetChannelByName(const std::string& name);

	// Clients Manager
	Client *GetUserByNickname(std::string nickname);

public:
	Server(int port, std::string password);
	void Run();
	bool Init();
	void Display();
};

#endif
