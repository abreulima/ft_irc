#include "Parser.hpp"
#include <iostream>

Type Parser::ProcessLine(std::string message)
{
	// std::cout << "<" << message << ">inside of the process<\n";
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
		std::vector<std::string> messages = Split(message, " ");
		if (messages.size() > 1)
		{
			mode.channel = messages.at(1);
		}
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
			// topic.topic = message.substr(message.find(':', 0));
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
