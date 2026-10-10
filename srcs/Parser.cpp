#include "Parser.hpp"
#include <iostream>

Type Parser::ProcessLine(std::string message)
{
	std::cout << "Client Says: " << message << std::endl;

	if (message.compare(0, 3, "CAP") == 0)
	{
		return (CAP);
	}
	else if (message.compare(0, 4, "NICK") == 0)
	{
		std::vector<std::string> messages = Split(message, " ");
		if (message.size() > 1)
		{
			nick.nickname = messages.at(1);
		}
		return NICK;
	}
	else if (message.compare(0, 4, "USER") == 0)
	{
		std::vector<std::string> messages = Split(message, " ");
		if (message.size() > 1)
		{
			user.username = messages.at(1);
		}
		return USER;
	}
	else if (message.compare(0, 7, "PRIVMSG") == 0)
	{
		std::vector<std::string> messages = Split(message, " ");
		if (message.size() > 1)
		{
			priv.target = messages.at(1);
		}
		priv.content = message.substr(message.find(":") + 1);
		return PRIVMSG;
	}
	else if (message.compare(0, 4, "KICK") == 0)
	{
		std::vector<std::string> messages = Split(message, " ");
		if (message.size() >= 3)
		{
			kick.channel = messages.at(1);
			kick.nickname = messages.at(2);
		}
		kick.reason = message.substr(message.find(":") + 1);
		return KICK;
	}
	else if (message.compare(0, 4, "JOIN") == 0)
	{
		std::vector<std::string> messages = Split(message, " ");
		if (message.size() > 1)
		{
			join.channel = messages.at(1);
		}
		return JOIN;
	}
	else if (message.compare(0, 3, "WHO") == 0)
	{
		std::vector<std::string> messages = Split(message, " ");
		if (messages.size() > 1)
		{
			who.channel = messages.at(1);
		}
		return WHO;
	}
	else if (message.compare(0, 4, "MODE") == 0)
	{

		int numArgs = 0;
		std::vector<std::string> messages = Split(message, " ");
		// mode
		if (messages.size() == 2)
		{
			mode.channel = messages.at(1);
			return MODE;
		}

		// 0     1     2   3
		// mode #canal +o ivan
		if (messages.size() > 2)
		{
			mode.modes = messages.at(2);
			for (size_t i = 0; i < messages.at(2).size(); i++)
			{
				if (messages.at(2)[i] == 'k' ||
					messages.at(2)[i] == 'l' || 
					messages.at(2)[i] == 'o')
				{
					numArgs++;
				}
			}
		}
	
		if (numArgs > messages.size() - 3)
			return ERROR;

		/*
		for (size_t i = 3; i < nu)
		{

		}
		*/
		return MODE;
	}
	else if (message.compare(0, 4, "QUIT") == 0)
	{
		return QUIT;
	}
	else if (message.compare(0, 5, "TOPIC") == 0)
	{
		std::cout << message << std::endl;
		std::vector<std::string> messages = Split(message, " ");
		if (messages.size() == 2)
		{
			topic.channel = messages.at(1);
			topic.shouldChange = false;
		}
		else if (messages.size() > 2)
		{
			topic.shouldChange = true;
			topic.channel = messages.at(1);
			topic.topic = message.substr(message.find(':', 0) + 1);
		}
		return TOPIC;
	}
	return (ERROR);
}

std::vector<std::string> Parser::Split(std::string lines)
{
	size_t start;
	size_t pos;

	std::string split = "\r\n";
	std::vector<std::string> res;
	start = 0;
	while ((pos = lines.find(split, start)) != std::string::npos)
	{
		res.push_back(lines.substr(start, pos - start));
		start = pos + split.size();
	}
	// res.push_back(lines.substr(start));
	//  std::cout << res.size() << "\n";
	return (res);
}

std::vector<std::string> Parser::Split(std::string lines, std::string split)
{
	size_t start;
	size_t pos;

	std::vector<std::string> res;
	start = 0;
	while ((pos = lines.find(split, start)) != std::string::npos)
	{
		res.push_back(lines.substr(start, pos - start));
		start = pos + split.size();
	}
	res.push_back(lines.substr(start));
	return (res);
}
