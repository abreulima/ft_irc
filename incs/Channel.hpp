#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <map>
# include <string>

struct Modes
{
	bool isInviteOnly;
	bool isTopicOpOnly;
	std::string password;
	int limit;
};

struct		Role
{
	bool	isOperator;
};

class Channel
{
  private:
	std::string _name;

  public:
	void Add(Client *c, Role r);
	void Remove(Client *c);
	std::map<Client *, Role> clients;

	size_t CountMembers();
	bool IsMember(Client *c);

	Client *GetUserByNickname(std::string nickname);

	Modes modes;

	// +itkl pass 10
	std::string GetModesString()
	{
		std::string res = "+";
		res += modes.isInviteOnly ? "i" : "";
		res += modes.isTopicOpOnly ? "t" : "";
		res += !modes.password.empty() ? "k" : "";
		res += modes.limit != 0 ? "l" : "";
		
		if (!modes.password.empty() || modes.limit != 0 )
		{
			res += " ";
			res += modes.password + " ";
			res += modes.limit;
		};
	}


};

#endif
