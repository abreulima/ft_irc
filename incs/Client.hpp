#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <set>

class Client
{
private:
    int _fd;
    std::string _username;
    std::string _nickname;
    std::string _hostname;
    std::set<std::string> _invites;

public:
    Client() {};
    //Client(int fd) : _fd(fd) {};
    //void SetFd(int fd);
    int GetFD()const ; 
    const std::string& GetUserName()const;
    const std::string& GetNickname()const;
    const std::string& GetHostName()const;
    bool useInvite(const std::string& ref);

    void SetFD(int value);
    void SetUsername(const std::string& value);
    void SetNickname(const std::string& value);
    void SetHostname(const std::string& value);
    void addInvite(const std::string& value);

    std::string GetPrefix();

};

#endif
