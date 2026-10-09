#include "Channel.hpp"

// OCF
Channel::Channel()
{
    modes.isInviteOnly = false;
    modes.isTopicOpOnly = true;
    modes.password = "";
    modes.limit = 0;
}

Channel::Channel(const Channel &copy) : _name(copy._name), modes(copy.modes), topic(copy.topic), clients(copy.clients)
{
    if (this == &copy)
        return;
}

Channel::~Channel()
{
}

const Channel &Channel::operator=(const Channel &copy)
{
    if (this == &copy)
        return *this;
    _name = copy._name;
    modes = copy.modes;
    clients = copy.clients;
    topic = copy.topic;
    return *this;
}

// getters
const std::string &Channel::getTopic() const
{
    return topic;
}

Client *Channel::GetUserByNickname(std::string nickname)
{
    std::map<Client *, Role>::iterator it;
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

    if (!modes.password.empty() || modes.limit != 0)
    {
        res += " ";
        res += modes.password + " ";
        res += modes.limit;
    };

    return res;
}

// setters
void Channel::setTopic(const std::string &ref)
{
    topic = ref;
}

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
