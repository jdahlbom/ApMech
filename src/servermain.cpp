#include "server/Server.h"

#ifdef WIN32
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main( int argc, char **argv )
#endif
{
    ap::server::Server *serverPtr = new ap::server::Server(50740);
    serverPtr->start();
}
