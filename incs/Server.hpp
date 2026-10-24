#ifndef SERVER_HPP
# define SERVER_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include "Parser.hpp"
# include "replies.hpp"

# include <map>
# include <vector>
# include <poll.h>
# include <stdint.h>
# include <iomanip>

# define SERVER_HOST	"42.pt"
# define SERVER_PREFIX	":" SERVER_HOST " "

# define RESET			"\033[0m\n"
# define RED			"\033[31m"
# define GREEN			"\033[32m"
# define YELLOW			"\033[33m"
# define BLUE			"\033[34m"

typedef std::map<std::string, Channel>::iterator	ChannelIterator;
typedef std::map<int, Client>::iterator				ClientIterator;
	
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
	void HandlePass(Client *c, Command::Pass data);
	void HandleUser(Client *c, Command::User data);
	void HandleInvite(Client *c, Command::Invite data);
	void HandleJoin(Client *c, Command::Join data);
	void HandlePart(Client *c, Command::Part data);
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
	void SendToChannels(std::vector<Channel> channels, std::string message, Client *exclude);
	void SendCode(Client c, int codeNum, std::string target, std::string message);
	void SendCode(Client c, int codeNum, std::string target1, std::string target2, std::string message);

	// Channel Manager
	std::vector<Channel> GetChannelsByClient(Client *c);
	Channel* GetChannelByName(const std::string& name);

	// Clients Manager
	Client *GetUserByNickname(std::string nickname);
	bool IsNicknameInUse(std::string nickname, Client* exclude);
	void TryRegister(Client *c);

	void DeleteEmptyChannels();
public:
	Server(std::string port, std::string password);
	void Run();
	bool Init();
	void Display();
};

#endif // SERVER_HPP
