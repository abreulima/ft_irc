#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <poll.h>

typedef struct pollfd pollfd_t;

class Client
{
private:
	std::string name;
	std::string nick;

public:
	Client();
	Client(std::string username, std::string nick);
	~Client();
};

#endif