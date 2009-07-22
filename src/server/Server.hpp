#ifndef AP_SERVER_SERVER_HPP
#define AP_SERVER_SERVER_HPP

#include "../net/netdata.h"
#include "../net/NetObject.h"
#include "../net/NetUser.h"
#include "../net/NetEvent.h"

namespace ap {
namespace server {

class Server
{
    public:
    Server(unsigned int port);
    void start();

    private:
    std::map<int,net::NetObject *>::iterator    p;
    std::map<int,net::NetObject *>::iterator    pProj;
    std::map<int, net::NetUser>::iterator       iUser;
    net::NetData                                *netdata;
    net::NetEvent                               event;
    unsigned long int                           nextupdate; // When we will send updates to clients the next time
    unsigned long int                           oldticks;   // These here for tracking time
    unsigned long int                           newticks;   // These here for tracking time
    float                                       dt;
    unsigned int                                mPort;
}; // class Server

} // namespace server
} // namespape ap

#endif
