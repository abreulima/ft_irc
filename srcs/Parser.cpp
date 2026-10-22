#include "Parser.hpp"
#include <iostream>

Type Parser::ProcessLine(std::string message)
{
	std::vector<std::string> messages = Split(message, " ");

	if (messages.at(0).compare("CAP") == 0)
	{
		return CAP;
	}
	else if (messages.at(0).compare("PASS") == 0)
	{
		if (messages.size() > 1)
		{
			pass.password = messages.at(1);
		}
		return PASS;
	}
	else if (messages.at(0).compare("NICK") == 0)
	{
		if (messages.size() > 1)
		{
			nick.nickname = messages.at(1);
		}
		return NICK;
	}
	else if (messages.at(0).compare("USER") == 0)
	{
		if (messages.size() > 1)
		{
			user.username = messages.at(1);
		}
		return USER;
	}
	else if (messages.at(0).compare("PRIVMSG") == 0)
	{
		if (messages.size() > 1)
		{
			priv.target = messages.at(1);
		}
		priv.content = message.substr(message.find(":") + 1);
		return PRIVMSG;
	}
	else if (messages.at(0).compare("KICK") == 0)
	{
		if (messages.size() >= 3)
		{
			kick.channel = messages.at(1);
			kick.nickname = messages.at(2);
		}
		kick.reason = message.substr(message.find(":") + 1);
		return KICK;
	}
	else if (messages.at(0).compare("PART") == 0)
	{
		if (messages.size() >= 2)
			part.channel = messages.at(1);
		if (messages.size() >= 3)
			part.reason = messages.at(2);
		return PART;
	}
	else if (messages.at(0).compare("JOIN") == 0)
	{
		if (messages.size() > 1)
			join.channel = messages.at(1);
		if (messages.size() > 2)
			join.password = messages.at(2);
		return JOIN;
	}
	else if (messages.at(0).compare("INVITE") == 0)
	{
		if (messages.size() > 2)
		{
			invite.nickname = messages.at(1);
			invite.channel = messages.at(2);
			return INVITE;
		}
		return ERROR;
	}
	else if (messages.at(0).compare("WHO") == 0)
	{
		if (messages.size() > 1)
		{
			who.channel = messages.at(1);
		}
		return WHO;
	}
	else if (messages.at(0).compare("MODE") == 0)
	{

		if (messages.size() < 2)
			return ERROR;

		mode.channel = messages.at(1);

		if (messages.size() == 2)
			return MODE;

		// 0    1      2
		// MODE #CANAL _
		// O Hexclient manda uma string vazia se o usuario digitar "/MODE"

		if (messages.at(2).empty())
		{
			mode.isOperation = false;
			return MODE;
		}
		else if (messages.at(2)[0] == '+')
		{
			mode.isOperation = true;
			mode.isAdding = true;
		}
		else if (messages.at(2)[0] == '-')
		{
			mode.isOperation = true;
			mode.isAdding = false;
		}
		else
		{
			return ERROR;
		}

		mode.modes = messages.at(2);

		// checa o numero de argumentos
		// (-+)o (operador), +k (password), +l (limit) precisam de argumentos
		size_t numArgs = 0;
		for (size_t i = 0; i < messages.at(2).size(); i++)
		{
			if (messages.at(2)[i] == 'l' && mode.isAdding)
				numArgs++;
			else if (messages.at(2)[i] == 'k' && mode.isAdding)
				numArgs++;
			else if (messages.at(2)[i] == 'o')
				numArgs++;
		}
		if (numArgs != messages.size() - 3)
			return ERROR;

		int argIndex = 3;
		for (size_t i = 0; i < messages.at(2).size(); i++)
		{
			char m = mode.modes[i];

			if (m == 'l' && mode.isAdding)
			{
				mode.args['l'] = messages.at(argIndex++);
			}
			else if (m == 'k' && mode.isAdding)
			{
				mode.args['k'] = messages.at(argIndex++);
			}
			else if (m == 'o')
			{
				mode.args['o'] = messages.at(argIndex++);
			}
		}
		return MODE;
	}
	else if (messages.at(0).compare("QUIT") == 0)
	{
		quit.reason = message.substr(message.find(':', 0) + 1);
		return QUIT;
	}
	else if (messages.at(0).compare("TOPIC") == 0)
	{
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
