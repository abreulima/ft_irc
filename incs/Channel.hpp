#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <map>
#include <string>

struct Modes
{
	bool isInviteOnly;
	bool isTopicOpOnly;
	std::string password;
	int limit;
};

struct Role
{
	bool isOperator;
};

class Channel
{
private:
	std::string _name;

public:
	Modes modes;
	std::string topic;
	std::map<Client *, Role> clients;

	// OCF
	Channel();
	Channel(const Channel &copy);
	~Channel();
	const Channel &operator=(const Channel &copy);

	// getters
	const std::string &getTopic() const;
	Client *GetUserByNickname(std::string nickname);
	std::string GetModesString();

	// setters
	void setTopic(const std::string &ref);

	// methods
	void Add(Client *c, Role r);
	void Remove(Client *c);
	size_t CountMembers();
	bool IsMember(Client *c);
};

#endif
