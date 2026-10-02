#include "Server.hpp"

int main()
{
    Server server;
    if (server.Init() == true)
        server.Run();
    return 1;
}
