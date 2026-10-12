#include "Server.hpp"
#include "Parser.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <cstdlib>


Server::Server(int port, std::string password) : port(port), password(password) 
{

}

void Server::Display()
{
	std::cout << "Server running at port " << port << "\n";
	std::cout << "Preferred client is HexChat." << "\n";
	return;
}

bool Server::Init()
{
	serverFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// TODO: Check socket return for errors

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
			int connectionFD = accept(serverFD, NULL, NULL);
			if (connectionFD >= 0)
			{
				fds.push_back((pollfd){connectionFD, POLLIN, 0});
				clients[connectionFD] = Client();
				clients[connectionFD].SetFD(connectionFD);
				clients[connectionFD].SetHostname("user.host"); // Change to User IP.
			}
		}

		for (size_t i = 1; i < fds.size();)
		{
			if (fds.at(i).revents & POLLIN)
			{
				// TODO: Manage buffering! Message could be incomplete.
				char incoming[513];
				int numBytes = recv(fds.at(i).fd, incoming, 512, 0);

				if (numBytes <= 0)
				{
					close(fds.at(i).fd);
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

		switch (cmd)
		{
			case CAP:		HandleCAP(&c, message);					break;
			case NICK:		HandleNick(&c, parser.nick);			break;
			case USER:		c.SetUsername(parser.user.username);	break;
			case PRIVMSG:	HandlePrivMsg(&c, parser.priv);			break;
			case JOIN:		HandleJoin(&c, parser.join);			break;
			case KICK:		HandleKick(&c, parser.kick);			break;
			case TOPIC:		HandleTopic(&c, parser.topic);			break;
			case ERROR:												break;
			case WHO:		HandleWho(&c, parser.who);				break;
			case MODE:		HandleMode(&c, parser.mode);			break;
			case QUIT:		HandleQuit(&c, parser.quit);			break;
			default:		std::cout << "UNKNOWN\n";				break;
		}

		if (cmd == ERROR)
			std::cerr << "\033[31m" << "User says: " << message << "\033[0m\n";
		else
			std::cerr << "\033[33m" << "User says: " << message << "\033[0m\n";
	}

	

}

void Server::HandleCAP(Client *c, std::string line)
{
	if (line.compare(0, 6, "CAP LS") == 0)
	{
		std::string response = ":42.pt CAP * LS 302 :";
		SendToClient(*c, response);
	}
	else if (line.compare(0, 7, "CAP END") == 0)
	{
		std::string response = ":42.pt 001 " + c->GetNickname() + " :";
		response += "Hello dear evaluator!";
		SendToClient(*c, response);
	}
}

void Server::HandleJoin(Client *c, Command::Join data)
{
	if (data.channel.empty() || data.channel.at(0) != '#')
		return;
	
	std::string res = c->GetPrefix() + " JOIN " + data.channel;

	Role role;
	if (channels[data.channel].CountMembers() > 0)
	{
		role.isOperator = false;
	}
	else
	{
		channels[data.channel] = Channel();
		role.isOperator = true;
	}

	channels[data.channel].Add(c, role);
	SendToChannel(channels[data.channel], res, 0);

	// this is ugly

	Channel &channel = channels[data.channel];
	std::string nickList;

	for (std::map<Client *, Role>::iterator it(channel.clients.begin()); it != channel.clients.end(); it++)
	{
		Client *member = it->first;
		Role role = it->second;

		nickList += role.isOperator ? "@" : "";
		nickList += member->GetNickname();
		nickList += " ";
	}

	SendToChannel(channels[data.channel], std::string("") + ":42.pt 353 " + c->GetNickname() + " = " + data.channel + " :" + nickList, 0);
	SendToChannel(channels[data.channel], std::string("") + ":42.pt 366 " + c->GetNickname() + " " + data.channel + " :End of /NAMES list", 0);

	if (channels[data.channel].getTopic().size() > 0)
	{
		std::string topic = ":42.pt " + RPL_TOPIC + " " + c->GetNickname() + " " + data.channel + " :" + channels[data.channel].getTopic();
		SendToClient(*c, topic);
	}
}

void Server::HandleTopic(Client *c, Command::Topic data)
{
	// if channel doesnt exists it segfaults
	if (channels.find(data.channel) == channels.end())
	{
		std::string res;
		res = ":42.pt " + ERR_NOSUCHCHANNEL + " " + c->GetNickname() + " " + data.channel + " :No such channel";
		SendToClient(*c, res);
		return;
	}

	if (channels.at(data.channel).clients.at(c).isOperator ||
		channels.at(data.channel).modes.isTopicOpOnly == false)
	{
		std::string res;
		res = ":42.pt 332 " + c->GetNickname() + " " + data.channel + " :" + data.topic;
		SendToChannel(channels[data.channel], res, NULL);
		channels.at(data.channel).setTopic(data.topic);
	}
	else
	{
		std::cerr << "is not operator\n";
	}
}

void Server::HandleKick(Client *c, Command::Kick data)
{
	Channel &channel = channels[data.channel];

	Role role = channel.clients.at(c);

	// 482 is ERR_CHANOPRIVSNEEDED.
	if (!role.isOperator)
	{
		std::string not_operator = ":42.pt 482 " + c->GetNickname() + " " + data.nickname + " :403 Forbidden - Not an Operator";
		SendToClient(*c, not_operator);
		return;
	}

	Client *kicked = channel.GetUserByNickname(data.nickname);
	// 401 is ERR_NOSUCHNICK
	if (!kicked)
	{
		std::string no_such_nick = ":42.pt 401 " + c->GetNickname() + " " + data.nickname + " :404 User Not Found";
		SendToClient(*c, no_such_nick);
		return;
		// :<server> 401 <requesting_nick> <target> :No such nick/channel
	}

	data.reason = data.reason.empty() ? c->GetNickname() : data.reason;

	std::string res = c->GetPrefix() + " KICK " + data.channel + " " + data.nickname + " :" + data.reason;
	SendToClient(*c, res);
	SendToChannel(channel, res, c);

	if (kicked)
		channel.Remove(kicked);
}

void Server::HandleNick(Client *c, Command::Nick data)
{
	// Check if nickname isn't being used
	
	/*
	std::map<int, Client>::iterator it;
	it = clients.begin();

	while (it != clients.end())
	{

		if (&it->second == c)
		{
			it++;
			continue;
		}

		if (it->second.GetNickname() == data.nickname)
		{
			std::string userNickname = c->GetNickname().empty() ? "*" : c->GetNickname();
			std::string res;
			res += ":42.pt " + ERR_NICKNAMEINUSE + " " + userNickname + " ";
			res += data.nickname + " :Nickname already in use.";
			SendToClient(*c, res);
			return ;
		}
		it++;
	}
	*/

	std::string res = c->GetPrefix() + " NICK " + data.nickname;
	std::vector<Channel *> channelsByMember = GetChannelsByClient(c);
	for(size_t i = 0; i < channelsByMember.size(); i++)
	{
		SendToChannel(*channelsByMember.at(i), res, NULL);
	}
	c->SetNickname(data.nickname);
}

void Server::HandlePrivMsg(Client *c, Command::Priv data)
{
	std::cout << "PRIVMSG from " << c->GetNickname() << " fd=" << c->GetFD() << std::endl;
	std::string res = c->GetPrefix() + " PRIVMSG " + data.target + " " + data.content;

	if (data.target.empty())
	{
		// TODO INVALID PARAM
		return ;
	}

	if (data.target.at(0) == '#')
	{
		std::map<std::string, Channel>::iterator it;
		it = channels.find(data.target);

		if (it == channels.end())
		{
			// TODO ERR_NOCHANNEL
			return ;
		}
		SendToChannel(channels[data.target], res, c);
	}
	else
	{
		std::map<int, Client>::iterator it; 
		it = clients.begin();

		while (it != clients.end())
		{
			if (it->second.GetNickname().compare(data.target) == 0)
			{
				SendToClient(it->second, res);
				return ;
			}
			it++;
		}
	}
	
	// TODO ERR_NOCLIENT
	
}

// per user
// :irc.server 352 Alice #Hello alice host1 irc.server Alice H@ :0 Alice
// :<server> 352 <requester> <channel> <user> <host> <server> <nick> <flags> :<hopcount> <realname>
void Server::HandleWho(Client *c, Command::Who data)
{

	Channel &channel = channels[data.channel];
	std::map<Client *, Role>::iterator it;
	it = channel.clients.begin();

	while (it != channel.clients.end())
	{
		Client *member = it->first;
		Role role = it->second;

		std::string res;
		res += ":42.pt 352 " + c->GetNickname() + " " + data.channel + " "; // 42.pt 352 Alice  #hello
		res += member->GetUserName() + " " + member->GetHostName() + " 42.pt " + member->GetNickname() + " ";
		res += role.isOperator ? "H@" : "H";
		res += " :0 realname";
		it++;

		SendToClient(*c, res);
	}

	//   :irc.server 315 Alice #Hello :End of /WHO list.
	std::string res = ":42.pt 315 " + c->GetNickname() + " " + data.channel + " :End of /WHO list.";
	SendToClient(*c, res);
}

void Server::HandleMode(Client *c, Command::Mode data)
{
	// :irc.server 324 <nick> #Hello +nt
	// 324 RPL_CHANNELMODEIS
	std::cout << data.channel << std::endl;
	Channel *channel = GetChannelByName(data.channel);
	
	if (!channel)
		return ;
	
	if (!data.isOperation)
	{
		std::string res = ":42.pt " + RPL_CHANNELMODEIS + c->GetNickname() + " " + data.channel + " " + channel->GetModesString();
		SendToClient(*c, res);
		return ;
	}

	if (!channel->IsOperator(c))
		return ;

	for (size_t i = 1 ; i < data.modes.size(); i++)
	{
		if (data.modes.at(i) == 'o')
		{
			Client *c = channel->GetUserByNickname(data.args.at('o'));
			if (!c)
				return ;
			
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
		else if(data.modes.at(i) == 'i')
		{
			data.isAdding 
			? channel->modes.isInviteOnly = true 
			: channel->modes.isInviteOnly = false;
		}
		else if (data.modes.at(i) == 'l')
		{
			data.isAdding
			? channel->SetLimit(data.args.at('l'))
			: channel->SetLimit(0);
		}
		else if (data.modes.at(i) == 'k')
		{
			data.isAdding 
			? channel->AddPassword(data.args.at('k'))
			: channel->RemovePassword();
		}
	}

	// Send it back to everyone
	std::string res = ":42.pt MODE " + data.channel + " ";
	res += data.modes;
	std::map<char, std::string>::iterator it;
	it = data.args.begin();
	while (it != data.args.end())
	{
		res += " ";
		res += it->second;
		it++;
	}
	std::cout << res << std::endl;
	SendToChannel(*channel, res, NULL);
}

void Server::HandleQuit(Client *c, Command::Quit data)
{
	(void)data;

	std::string res = c->GetPrefix() + " QUIT :Bye";
	// Send message to everyone the member is leaving
	std::vector<Channel *> channelsByMember = GetChannelsByClient(c);
	for(size_t i = 0; i < channelsByMember.size(); i++)
	{
		channelsByMember.at(i)->Remove(c);
		SendToChannel(*channelsByMember.at(i), res, c);
	}
	// If the channel is empty it should be removed
	std::map<std::string, Channel>::iterator it;
	it = channels.begin();
	while (it != channels.end())
	{
		if (it->second.CountMembers() == 0)
			channels.erase(it++);
		else
			++it;
	}
}

void Server::SendToClient(Client c, std::string message)
{
	std::cerr << "\033[32m" << "Server says: " << message << "\033[0m\n";
	message += "\r\n";
	send(c.GetFD(), message.c_str(), message.size(), 0);
}

void Server::SendToChannel(Channel c, std::string message, Client *exclude)
{
	std::map<Client *, Role>::iterator it;	
	for (it = c.clients.begin(); it != c.clients.end(); it++)
	{
		if (it->first == exclude)
			continue;
		SendToClient(*it->first, message);
	}
}

// Return all the channels' pointer the client is a member.
std::vector<Channel*> Server::GetChannelsByClient(Client *c)
{
	std::vector<Channel*> res;

	std::map<std::string, Channel>::iterator it;
	it = channels.begin();

	while (it != channels.end())
	{
		Channel channel = it->second;
		if (channel.IsMember(c))
			res.push_back(&it->second);
		it++;
	}
	return res;
}

Channel* Server::GetChannelByName(const std::string& name)
{
	std::map<std::string, Channel>::iterator it;
	it = channels.find(name);

	if (it != channels.end())
		return &it->second;
	else
		return NULL;
}