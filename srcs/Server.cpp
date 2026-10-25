#include "Server.hpp"
#include "Parser.hpp"
#include "replies.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <cstdlib>
#include <sstream>
#include <string>

static uint16_t parsePort(const std::string& s)
{
	long port;
	if (s.empty()) // TODO: check invalid chars
		throw std::invalid_argument("port should be an unsigned numeric");
	
	char* endptr;
	port = strtol(s.c_str(), &endptr, 10);
	// TODO: check strtol errno

	if (port < 1 || port > 65535)
		throw std::out_of_range("port should be bigger than 0 and lower or equal to 65535");
	
	return static_cast<uint16_t>(port);
}

Server::Server(std::string port, std::string password): port(parsePort(port)), password(password) {}

void Server::Display()
{
	std::cout << "Server running at port " << port << " with password " << password << std::endl;
	std::cout << "Preferred client is HexChat." << std::endl;
	return;
}

bool Server::Init()
{
	serverFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverFD == -1) {
		perror("Can't create the socket.");
		return false;
	}

	int opt = 1;
	setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	sockaddr_in serverAddress;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_family = AF_INET;

	// fcntl(serverFD, F_SETFL, O_NONBLOCK);

	int res = bind(serverFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	if (res == -1)
	{
		perror("BIND Error");
		return false;
	}

	listen(serverFD, 1024);
	// TODO: Check listen return for errors

	fds.push_back((pollfd){serverFD, POLLIN, 0});

	Display();
	isRunning = true;

	return true;
}

void Server::Run()
{
	while (isRunning)
	{
		poll(fds.data(), fds.size(), -1);

		// Servidor recebeu algo!
		if (fds.at(0).revents & POLLIN)
		{
			struct sockaddr_in client_addr;
			socklen_t client_len = sizeof(client_addr);

			int connectionFD = accept(serverFD, (struct sockaddr *)&client_addr, &client_len);
			
			if (connectionFD >= 0)
			{
				std::string clientIPV4 = inet_ntoa(client_addr.sin_addr);

				fds.push_back((pollfd){connectionFD, POLLIN, 0});
				clients[connectionFD] = Client();
				clients[connectionFD].SetFD(connectionFD);
				clients[connectionFD].SetHostname(clientIPV4);
			}
		}

		for (size_t i = 1; i < fds.size();)
		{
			if (fds.at(i).revents & POLLIN)
			{
				// TODO: Manage buffering! Message could be incomplete.
				char incoming[513];
				int numBytes = recv(fds.at(i).fd, incoming, 512, 0);

				// TODO: Not sure but I think we should handle == 0 apart from errors.
				// If client disconnects, it should remove that client of all channels
				// and if the channel is empty after that it should be removed as well.
				if (numBytes <= 0)
				{
					int fd = fds.at(i).fd;

					close(fd);
					clients.erase(fd);
					fds.erase(fds.begin() + 1);
					continue;
				}
				else
				{
					Parser parser;
					std::vector<std::string> messages = parser.Split(std::string(incoming, numBytes));
					handleCommands(i, messages);
				}
			}
			i++;
		}
	}
}

void Server::handleCommands(size_t fd_id, std::vector<std::string> &messages)
{
	Parser parser;

	Client &c = clients[fds.at(fd_id).fd];

	for (size_t j = 0; j < messages.size(); ++j)
	{
		std::string message = messages.at(j);
		Type cmd = parser.ProcessLine(message);

		if (cmd == ERROR)
			std::cerr << RED << "User (" + c.GetNickname() + ") : " << message << RESET << std::endl;
		else
			std::cerr << YELLOW << "User (" + c.GetNickname() + ") : " << message << RESET << std::endl;

		if (!c.IsRegistered() && (cmd != CAP && cmd != PASS && cmd != NICK && cmd != USER && cmd != QUIT)) {
			SendCode(c, ERR_NOTREGISTERED, "", "Not yet registered!");
			return;
		} else if (c.IsRegistered() && (cmd == USER || cmd == PASS)) {
			SendCode(c, ERR_ALREADYREGISTERED, "Already Registered.");
			return;
		}

		switch (cmd)
		{
			case CAP:		HandleCAP(&c, message);				break;
			case PASS:		HandlePass(&c, parser.pass);			break;
			case NICK:		HandleNick(&c, parser.nick);			break;
			case USER:		HandleUser(&c, parser.user);			break;
			case PRIVMSG:	HandlePrivMsg(&c, parser.priv);		break;
			case INVITE:	HandleInvite(&c, parser.invite);		break;
			case JOIN:		HandleJoin(&c, parser.join);			break;
			case PART:		HandlePart(&c, parser.part);			break;
			case KICK:		HandleKick(&c, parser.kick);			break;
			case TOPIC:		HandleTopic(&c, parser.topic);			break;
			case WHO:		HandleWho(&c, parser.who);				break;
			case MODE:		HandleMode(&c, parser.mode);			break;
			case QUIT:		HandleQuit(&c, parser.quit);			break;

			case ERROR:													break;

			default:		SendCode(c, ERR_UNKNOWNCOMMAND, c.GetNickname(), "Unknown Command.");
		}
	}
}

void Server::HandleCAP(Client *c, std::string line)
{
	if (line.compare(0, 6, "CAP LS") == 0)
	{
		c->SetIsCapEnded(false);
		SendToClient(*c, std::string(SERVER_PREFIX) + "CAP * LS :");
	}
	else if (line.compare(0, 7, "CAP END") == 0)
	{	
		c->SetIsCapEnded(true);
		TryRegister(c);
	}
}

void Server::HandlePass(Client *c, Command::Pass data)
{
	c->SetPassword(data.password);
	TryRegister(c);
}

void Server::HandleNick(Client *c, Command::Nick data)
{

	if (c->GetNickname() == data.nickname)
		return ;
		
	if (IsNicknameInUse(data.nickname, c))
	{
		SendCode(*c, ERR_NICKNAMEINUSE, data.nickname, "Nickname is taken.");
		return ;
	}

	std::string res = c->GetPrefix() + " NICK :" + data.nickname;

	if (c->IsRegistered())
	{
		SendToClient(*c, res);
		SendToChannels(GetChannelsByClient(c), res, NULL);
	}

	c->SetNickname(data.nickname);

	TryRegister(c);
}

void Server::HandleUser(Client *c, Command::User data)
{
	c->SetUsername(data.username);
	TryRegister(c);
}

// TODO: Missing Error Handling
void Server::HandleJoin(Client *c, Command::Join data)
{
	if (data.channel.empty() || data.channel.at(0) != '#')
		return;

	bool isNew = (channels.find(data.channel) == channels.end());

	if (isNew) {
		channels.insert(make_pair(data.channel, Channel(data.channel)));
	}

	Channel *channel = GetChannelByName(data.channel);
	
	// TODO: needs error handling
	if (channel->modes.isInviteOnly && !c->useInvite(data.channel))
		return ;
	if (!channel->modes.password.empty() && data.password != channel->modes.password)
		return ;

	Role role;
	role.isOperator = isNew ? true : false;
	channel->Add(c, role);

	std::string res = c->GetPrefix() + " JOIN " + data.channel;
	SendToChannel(*channel, res, NULL);

	std::string nickList;

	for (ClientRoleIterator it(channel->clients.begin()); it != channel->clients.end(); it++)
	{
		Client *member = it->first;
		Role role = it->second;

		nickList += role.isOperator ? "@" : "";
		nickList += member->GetNickname();
		nickList += " ";
	}

	SendToChannel(*channel, std::string(SERVER_PREFIX) + ReplyCodeToString(RPL_NAMREPLY) + " " + c->GetNickname() + " = " + data.channel + " :" + nickList, 0);
	SendToChannel(*channel, std::string(SERVER_PREFIX) + ReplyCodeToString(RPL_ENDOFNAMES) + " " + c->GetNickname() + " " + data.channel + " :End of /NAMES list", 0);

	if (channel->getTopic().size() > 0)
	{
		std::string topic = std::string(SERVER_PREFIX) + ReplyCodeToString(RPL_TOPIC) + " " + c->GetNickname() + " " + data.channel + " :" + channel->getTopic();
		SendToClient(*c, topic);
	}
}

// TODO: Handle multiple channels, loop trought add channels passed between commas
void Server::HandlePart(Client *c, Command::Part data)
{
	Channel *channel = GetChannelByName(data.channel);
	if (!channel)
	{
		SendCode(*c, ERR_NOSUCHCHANNEL, c->GetNickname(), data.channel, "No such channel");	
		return;
	}

	if (!channel->IsMember(c))
	{
		SendCode(*c, ERR_NOTONCHANNEL, c->GetNickname(), data.channel, "User not on Channel.");
		return;
	}
	
	std::string res = c->GetPrefix() + " PART " + data.channel + " :" + data.reason;
	SendToChannel(*channel, res, NULL);

	channel->Remove(c);
	DeleteEmptyChannel(*channel);
}

// TODO: Error Handling
void Server::HandleTopic(Client *c, Command::Topic data)
{

	Channel *channel = GetChannelByName(data.channel);
	if (!channel)
	{
		SendCode(*c, ERR_NOSUCHCHANNEL, c->GetNickname(), data.channel, "No such channel");	
		return;
	}

	if (channel->clients.at(c).isOperator || channel->modes.isTopicOpOnly == false)
	{
		std::string res = std::string(SERVER_PREFIX) + ReplyCodeToString(RPL_TOPIC) + " " + c->GetNickname() + " " + data.channel + " :" + data.topic;
		SendToChannel(*channel, res, NULL);
		channel->setTopic(data.topic);
	}
	else
	{
		std::cerr << "is not operator" << std::endl;
	}
}

void Server::HandleKick(Client *c, Command::Kick data)
{
	Channel *channel = GetChannelByName(data.channel);
	if (!channel)
	{
		SendCode(*c, ERR_NOSUCHCHANNEL, data.nickname, data.channel, "No such channel");	
		return;
	}

	if (!channel->IsMember(c))
	{
		SendCode(*c, ERR_NOTONCHANNEL, data.nickname, "You're not on that channel.");
		return;
	}

	Role role = channel->clients.at(c);
	if (!role.isOperator)
	{
		SendCode(*c, ERR_CHANOPRIVSNEEDED, data.channel, "Not an Operator.");
		return;
	}

	Client *kicked = channel->GetUserByNickname(data.nickname);
	if (!channel->IsMember(kicked))
	{
		SendCode(*c, ERR_USERNOTINCHANNEL, data.nickname, data.channel, "User not in channel.");
		return;
	}

	channel->Remove(kicked);
	DeleteEmptyChannel(*channel);
	
	data.reason = data.reason.empty() ? c->GetNickname() : data.reason;
	std::string res = c->GetPrefix() + " KICK " + data.channel + " " + data.nickname + " :" + data.reason;
	SendToChannel(*channel, res, NULL);
}

void Server::HandleInvite(Client *c, Command::Invite data)
{
	Channel *channel = GetChannelByName(data.channel);
	if (!channel)
	{
		SendCode(*c, ERR_NOSUCHCHANNEL, c->GetNickname(), data.channel, "No such channel");	
		return;
	}
 
	// Channel is Invite Only and Sender isn't operator.
	if (channel->modes.isInviteOnly && !channel->IsOperator(c))
	{
		SendCode(*c, ERR_CHANOPRIVSNEEDED, data.channel, "You aren't allowed.");
		return ;
	}

	Client *invited = Server::GetUserByNickname(data.nickname);

	if (!invited)
		return;
	
	invited->addInvite(data.channel);

	// TODO: Issuer and Invited clients should receive different messages.
	// TODO: Issuer should receive RPL_INVITING instead of INVITE
	std::string msg = c->GetPrefix() + " INVITE " + data.nickname + " " + data.channel;
	SendToClient(*c, msg);
	SendToClient(*invited, msg);
}

void Server::HandlePrivMsg(Client *c, Command::Priv data)
{
	std::string res = c->GetPrefix() + " PRIVMSG " + data.target + " " + data.content;

	if (data.target.empty())
	{
		// TODO INVALID PARAM
		return;
	}

	if (data.target.at(0) == '#')
	{
		Channel *channel = GetChannelByName(data.target);
		if (!channel)
		{
			SendCode(*c, ERR_NOSUCHCHANNEL, c->GetNickname(), data.target, "No such channel");	
			return;
		}
		SendToChannel(*channel, res, c);
	}
	else
	{
		Client* receiver = GetUserByNickname(data.target);
		if (receiver)
			SendToClient(*receiver, res);
	}

	// TODO ERR_NOCLIENT
}

// per user
// :irc.server 352 Alice #Hello alice host1 irc.server Alice H@ :0 Alice
// :<server> 352 <requester> <channel> <user> <host> <server> <nick> <flags> :<hopcount> <realname>
// TODO OR NOT TODO: WHO handles nicknames as well as channels.
void Server::HandleWho(Client *c, Command::Who data)
{
	Channel *channel = GetChannelByName(data.channel);
	if (!channel)
	{
		SendCode(*c, ERR_NOSUCHCHANNEL, c->GetNickname(), data.channel, "No such channel");	
		return;
	}

	ClientRoleIterator it = channel->clients.begin();

	while (it != channel->clients.end())
	{
		Client *member = it->first;
		Role role = it->second;

		std::string res;
		res += std::string(SERVER_PREFIX) + ReplyCodeToString(RPL_WHOREPLY) + " " + c->GetNickname() + " " + data.channel + " "; // 42.pt 352 Alice  #hello
		res += member->GetUserName() + " " + member->GetHostName() + " 42.pt " + member->GetNickname() + " ";
		res += role.isOperator ? "H@" : "H";
		res += " :0 realname";
		it++;

		SendToClient(*c, res);
	}

	//   :irc.server 315 Alice #Hello :End of /WHO list.
	std::string res = std::string(SERVER_PREFIX) + ReplyCodeToString(RPL_ENDOFWHO) + " " + c->GetNickname() + " " + data.channel + " :End of /WHO list.";
	SendToClient(*c, res);
}

// :irc.server 324 <nick> #Hello +nt
// 324 RPL_CHANNELMODEIS
// std::cout << data.channel << std::endl;
void Server::HandleMode(Client *c, Command::Mode data)
{
	Channel *channel = GetChannelByName(data.channel);
	if (!channel)
	{
		SendCode(*c, ERR_NOSUCHCHANNEL, c->GetNickname(), data.channel, "No such channel");	
		return;
	}

	if (!data.isOperation)
	{
		std::string res = std::string(SERVER_PREFIX) + ReplyCodeToString(RPL_CHANNELMODEIS) + " " + c->GetNickname() + " " + data.channel + " " + channel->GetModesString();
		SendToClient(*c, res);
		return;
	}

	if (!channel->IsOperator(c))
		return;

	for (size_t i = 1; i < data.modes.size(); i++)
	{
		if (data.modes.at(i) == 'o')
		{
			Client *c = channel->GetUserByNickname(data.args.at('o'));
			if (!c)
				return;

			data.isAdding
				? channel->AddOperator(c)
				: channel->RemoveOperator(c);
		}
		else if (data.modes.at(i) == 't')
		{
			data.isAdding
				? channel->modes.isTopicOpOnly = true
				: channel->modes.isTopicOpOnly = false;
		}
		else if (data.modes.at(i) == 'i')
		{
			data.isAdding
				? channel->modes.isInviteOnly = true
				: channel->modes.isInviteOnly = false;
		}
		else if (data.modes.at(i) == 'l')
		{
			if (data.isAdding) {
				int limit = std::atoi(data.args.at('l').c_str());
				
				if (limit > 0)
					channel->SetLimit(limit);
				else
					return;
			} else {
				channel->SetLimit(0);
			}
		}
		else if (data.modes.at(i) == 'k')
		{
			data.isAdding
				? channel->AddPassword(data.args.at('k'))
				: channel->RemovePassword();
		}
	}

	// Send it back to everyone
	std::string res = std::string(SERVER_PREFIX) + "MODE " + data.channel + " ";
	res += data.modes;

	for (std::map<char, std::string>::iterator it = data.args.begin(); it != data.args.end(); it++)
		res += " " + it->second;

	SendToChannel(*channel, res, NULL);
}

void Server::HandleQuit(Client *c, Command::Quit data)
{
	std::string res = c->GetPrefix() + " QUIT :" + data.reason;
	std::vector<Channel> channelsByMember = GetChannelsByClient(c);
	for (std::vector<Channel>::iterator it = channelsByMember.begin(); it != channelsByMember.end(); it++)
	{
		it->Remove(c);
		SendToChannel(*it, res, c);
		DeleteEmptyChannel(*it);
	}
}

void Server::SendToClient(Client c, std::string message)
{
	std::cerr << GREEN << "Server says: " << message << RESET << std::endl;
	message += CRLF;
	send(c.GetFD(), message.c_str(), message.size(), 0);
}

void Server::SendCode(Client c, int codeNum, std::string message)
{
	std::stringstream ss;
	ss << std::setw(3) << std::setfill('0') << codeNum;

	std::string nick = c.GetNickname();
	if (nick.empty())
		nick = "*";

	std::string res = std::string(SERVER_PREFIX) + ss.str() + " ";
	res += nick;

	res += " :" + message;

	SendToClient(c, res);
}

void Server::SendCode(Client c, int codeNum, std::string target, std::string message)
{
	std::stringstream ss;
	ss << std::setw(3) << std::setfill('0') << codeNum;

	std::string nick = c.GetNickname();
	if (nick.empty())
		nick = "*";

	std::string res = std::string(SERVER_PREFIX) + ss.str() + " ";
	res += nick;

	if (!target.empty())
		res += " " + target;

	res += " :" + message;

	SendToClient(c, res);
}

void Server::SendCode(Client c, int codeNum, std::string target1, std::string target2, std::string message)
{
	std::stringstream ss;
	ss << std::setw(3) << std::setfill('0') << codeNum;

	std::string nick = c.GetNickname();
	if (nick.empty())
		nick = "*";

	std::string res = std::string(SERVER_PREFIX) + ss.str() + " ";
	res += nick;

	if (!target1.empty())
		res += " " + target1;
	
	if (!target2.empty())
		res += " " + target2;

	res += " :" + message;

	SendToClient(c, res);
}

void Server::SendToChannel(Channel c, std::string message, Client *exclude)
{
	for (ClientRoleIterator it = c.clients.begin(); it != c.clients.end(); it++)
	{
		if (it->first == exclude)
			continue;
		SendToClient(*it->first, message);
	}
}

void Server::SendToChannels(std::vector<Channel> channels, std::string message, Client *exclude)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		SendToChannel(channels.at(i), message, exclude);
	}
}

// Return all the channels' pointer the client is a member.
std::vector<Channel> Server::GetChannelsByClient(Client *c)
{
	std::vector<Channel> res;

	ChannelIterator it = channels.begin();

	while (it != channels.end())
	{
		Channel channel = it->second;
		if (channel.IsMember(c))
			res.push_back(channel);
		it++;
	}
	return res;
}

Channel *Server::GetChannelByName(const std::string &name)
{
	ChannelIterator found = channels.find(name);

	if (found != channels.end())
		return &found->second;
	else
		return NULL;
}

// TODO: unit test
Client *Server::GetUserByNickname(std::string nickname)
{
	ClientIterator it = clients.begin();
	while (it != clients.end())
	{
		Client *member = &it->second;
		if (member->GetNickname() == nickname)
			return member;
		it++;
	}
	return NULL;
}

bool Server::IsNicknameInUse(std::string nickname, Client* exclude)
{
	ClientIterator it = clients.begin();

	while (it != clients.end())
	{
		Client *member = &it->second;
		
		if (member != exclude &&
			member->GetNickname() == nickname)
			return true;
		
		it++;
	}
	return false;
}

void Server::TryRegister(Client *c)
{
	if (c->IsRegistered())
		return ;
	
	if (c->GetNickname().empty())
		return ;
	
	if (c->GetUserName().empty())
		return ;

	if (IsNicknameInUse(c->GetNickname(), c))
		return ;

	if (!c->IsCapEnded())
		return ;

	if (!password.empty() && c->GetPassword().compare(password) != 0) {
		SendCode(*c, ERR_PASSWDMISMATCH, "Password mismatch.");
		return;
	}

	c->SetRegistered(true);

	SendCode(*c, RPL_WELCOME, "", "Hello dear evaluator!");
}

// Removes the channel from the map if it's empty
void Server::DeleteEmptyChannel(Channel& channel)
{
	if (channel.CountMembers() == 0)
		channels.erase(channel.getName());
}

// Removes empty channels from the map
void Server::DeleteEmptyChannels()
{
	for (ChannelIterator it = channels.begin(); it != channels.end(); it++)
	{
		DeleteEmptyChannel(it->second);
	}
}
