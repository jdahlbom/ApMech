#include "server/Server.hpp"

int main(int argc, char* argv[])
{
    ap::server::Server *serverPtr = new ap::server::Server(50740);
    serverPtr->start();
}
