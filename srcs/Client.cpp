#include "Client.hpp"

#include <iostream>

int Client::GetFD() const { return _fd; };
const std::string &Client::GetUserName() const { return _username; }
const std::string &Client::GetNickname() const { return _nickname; }
const std::string &Client::GetHostName() const { return _hostname; }
bool Client::useInvite(const std::string &ref)
{
	if (_invites.find(ref) != _invites.end())
	{
		_invites.erase(ref);
		return true;
	}
	return false;
}

void Client::SetFD(int value) { _fd = value; }
void Client::SetUsername(const std::string &value) { _username = value; }
void Client::SetNickname(const std::string &value) { _nickname = value; }
void Client::SetHostname(const std::string &value) { _hostname = value; }
void Client::addInvite(const std::string &value)
{
	_invites.insert(value);
}

std::string Client::GetPrefix() //:<nickname>!<username>@<hostname>
{
	return std::string(
		":" + this->GetNickname() + "!" + this->GetUserName() + "@" + this->GetHostName());
}
