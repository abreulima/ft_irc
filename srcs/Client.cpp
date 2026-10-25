#include "Client.hpp"

Client::Client() : _isRegistered(false), _isCapEnded(false) {}

int Client::GetFD() const
{
	return (_fd);
};
const std::string &Client::GetUserName() const
{
	return (_username);
}
const std::string &Client::GetNickname() const
{
	return (_nickname);
}
const std::string &Client::GetHostName() const
{
	return (_hostname);
}
const std::string &Client::GetPassword() const
{
	return (_password);
}
bool Client::useInvite(const std::string &ref)
{
	if (_invites.find(ref) != _invites.end())
	{
		_invites.erase(ref);
		return (true);
	}
	return (false);
}

bool Client::IsRegistered()
{
	return _isRegistered;
}

void Client::SetRegistered(bool value)
{
	_isRegistered = value;
}

void Client::SetFD(int value)
{
	_fd = value;
}
void Client::SetUsername(const std::string &value)
{
	_username = value;
}
void Client::SetNickname(const std::string &value)
{
	_nickname = value;
}
void Client::SetHostname(const std::string &value)
{
	_hostname = value;
}
void Client::SetPassword(const std::string &value)
{
	_password = value;
}
void Client::addInvite(const std::string &value)
{
	_invites.insert(value);
}

bool Client::IsCapEnded()
{
	return _isCapEnded;
}
void Client::SetIsCapEnded(bool value)
{
	_isCapEnded = value;
}

std::string Client::GetPrefix() //:<nickname>!<username>@<hostname>
{
	return (std::string(":" + this->GetNickname() + "!" + this->GetUserName()
			+ "@" + this->GetHostName()));
}
