#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
private:
    int _fd;
    std::string _username;
    std::string _nickname;
    std::string _hostname;

public:
    Client() {};
    //Client(int fd) : _fd(fd) {};
    //void SetFd(int fd);
    int GetFD(); 
    std::string GetUserName();
    std::string GetNickname();
    std::string GetHostName();

    void SetFD(int value);
    void SetUsername(std::string value);
    void SetNickname(std::string value);
    void SetHostname(std::string value);

    std::string GetPrefix();

};

#endif