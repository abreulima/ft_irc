#include "Server.hpp"

int main()
{

    Server serv;
    
    if (serv.Init() == false)
        return 1;

    serv.Run();

    return 0;
}
