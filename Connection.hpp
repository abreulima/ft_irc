#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include <poll.h>

typedef struct pollfd pollfd_t;

class Connection
{
private:
	std::string name;
	std::string nick;

public:
	Connection();
	Connection(std::string username, std::string nick);
	~Connection();

	std::string GetName();
	std::string GetNick();
};

#endif