
#include "Server.hpp"

int main(/* int argc, char const *argv[] */)
{
    Server server(6667, "");
    
    if (server.Init() == true)
        server.Run();
    return 1;
}
