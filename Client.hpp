#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client
{
private:
    int _fd;
public:
    void SetFd(int fd);
    int GetFd(int fd);        
};

#endif