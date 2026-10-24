#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <map>
# include <string>

struct Modes
{
	bool isMemberOnly;
	bool isInviteOnly;
	bool isTopicOpOnly;
	std::string password;
	int limit;
};

struct Role
{
	bool isOperator;
};

typedef std::map<Client *, Role>::iterator	ClientRoleIterator;

class Channel
{
private:
	std::string _name;

public:
	Modes modes;
	std::string topic;
	std::map<Client *, Role> clients;

	// OCF
	Channel(std::string name);
	Channel(const Channel &copy);
	~Channel();
	const Channel &operator=(const Channel &copy);

	// getters
	const std::string &getName() const;
	const std::string &getTopic() const;
	Client *GetUserByNickname(std::string nickname);
	std::string GetModesString();

	// setters
	void setName(const std::string &ref);
	void setTopic(const std::string &ref);

	// methods
	void Add(Client *c, Role r);
	void Remove(Client *c);
	size_t CountMembers();
	bool IsMember(Client *c);
	bool IsOperator(Client *c);

	void AddOperator(Client *c);
	void RemoveOperator(Client *c);

	void AddPassword(std::string c);
	void RemovePassword();

	void SetLimit(int limit);
	// void SetLimit(const std::string& limit);
};

#endif // CHANNEL_HPP
