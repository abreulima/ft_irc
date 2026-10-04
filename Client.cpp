#include "Client.hpp"

#include <iostream>

int Client::GetFD()							{ return _fd; };
std::string Client::GetUserName()			{ return _username; }
std::string Client::GetNickname()		   { return _nickname; }
std::string Client::GetHostName()		   { return _hostname; }
void Client::SetFD(int value)			   { _fd = value; }
void Client::SetUsername(std::string value) { _username = value; }
void Client::SetNickname(std::string value) { _nickname = value; }
void Client::SetHostname(std::string value) { _hostname = value; }

std::string Client::GetPrefix()
{
	//:<nickname>!<username>@<hostname>
	std::string prefix;
	prefix = ":" + this->GetNickname() + "!" + this->GetUserName() + "@" + this->GetHostName();
	return prefix;
}

/* 

bybxs-95-92-143-217.run.pinggy-free.link:42205


*/