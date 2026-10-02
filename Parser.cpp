#include "Parser.hpp"
#include <iostream>

CommandType Parser::ProcessLine(std::string message)
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
			commandNickData.nickname = messages.at(1);
		}
		return (NICK);
	}
	else if (message.compare(0, 4, "USER") == 0)
	{
		std::vector<std::string> messages = Split(message, " ");
		if (message.size() > 1)
		{
			commandUserData.username = messages.at(1);
		}
		return (USER);
	}
	else if (message.compare(0, 7, "PRIVMSG") == 0)
	{
		std::vector<std::string> messages = Split(message, " ");
		if (message.size() > 1) { commandPrivData.target = messages.at(1); }
        commandPrivData.content = message.substr(message.find(":") + 1);
		return (PRIVMSG);
	}
	// join #channel
	else if (message.compare(0, 4, "JOIN") == 0)
	{
		std::vector<std::string> messages = Split(message, " ");
		if (message.size() > 1)
		{
			commandJoinData.channel = messages.at(1);
		}
		return (JOIN);
	}
	else if (message.compare(0, 4, "QUIT") == 0 )
	{
		return QUIT;
	}
	return (ERROR);
}

std::vector<std::string> Parser::Split(std::string lines)
{
	size_t	start;
	size_t	pos;

	std::string split = "\r\n";
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

std::vector<std::string> Parser::Split(std::string lines, std::string split)
{
	size_t	start;
	size_t	pos;

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
