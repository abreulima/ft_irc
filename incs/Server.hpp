#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
#include "Client.hpp"
#include "Parser.hpp"

#include <map>
#include <vector>
#include <poll.h>
#include <stdint.h>
#include <iomanip>

#define RPL_WELCOME				001
#define RPL_TOPIC 				std::string("332")
#define RPL_CHANNELMODEIS 		std::string("324")
#define ERR_NEEDMOREPARAMS 		std::string("461")
#define ERR_NOSUCHCHANNEL 		403
#define ERR_CHANOPRIVSNEEDED 	482
#define ERR_NICKNAMEINUSE 		433
#define ERR_NOSUCHNICK			401
#define SERVER_HOST 			std::string("42.pt")

class Server
{
private:
	std::vector<pollfd>				fds;
	std::map<std::string, Channel>	channels;
	std::map<int, Client>			clients;
	int 							serverFD;
	uint16_t						port;
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

	// Senders
	void SendToClient(Client c, std::string message);
	void SendToChannel(Channel c, std::string message, Client *exclude);
	void SendToChannels(std::vector<Channel *> channels, std::string message, Client *exclude);
	void SendCode(Client c, int codeNum, std::string channel, std::string message);

	// Channel Manager
	std::vector<Channel*> GetChannelsByClient(Client *c);
	Channel* GetChannelByName(const std::string& name);

	// Clients Manager
	Client *GetUserByNickname(std::string nickname);
	bool IsNicknameInUse(std::string nickname, Client* exclude);
	void TryRegister(Client *c);


public:
	Server(std::string port, std::string password);
	void Run();
	bool Init();
	void Display();
};

#endif
