#include "Channel.hpp"

void Channel::Add(Client *c, Role r)
{
    clients[c] = r;
}

void Channel::Remove(Client *c)
{
    clients.erase(c);
}

size_t Channel::CountMembers()
{
    return clients.size();
}

bool Channel::IsMember(Client *c)
{
    if (clients.find(c) != clients.end())
        return true;
    return false;
}

Client *Channel::GetUserByNickname(std::string nickname)
{
    std::map<Client*, Role>::iterator it;
	it = clients.begin();

	while (it != clients.end())
	{
		Client *member = it->first; 
		
        if (member->GetNickname() == nickname)
            return member;
	}
    return NULL;
}

// +itkl pass 10
std::string Channel::GetModesString()
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
    
    return res;
}
