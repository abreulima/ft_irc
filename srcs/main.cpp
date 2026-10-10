
#include "Server.hpp"

int main(/* int argc, char const *argv[] */)
{
    Server server;
    
    if (server.Init() == true)
        server.Run();
    return 1;
}
