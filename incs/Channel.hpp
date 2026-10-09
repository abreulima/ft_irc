#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <map>
# include <string>

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

};

#endif
