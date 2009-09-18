#ifndef AP_SERVER_SERVER_HPP
#define AP_SERVER_SERVER_HPP

#include "../net/netdata.h"
#include "../net/NetObject.h"
#include "../net/NetUser.h"
#include "../net/NetEvent.h"

#include "../Mech.h"
#include "../MovingObject.hpp"
#include "../types.h"

namespace ap {
namespace server {

class Server
{
    public:
    Server(uint32 port);
    void start();

    private:
    ap::net::NetData    *netdata;
    uint64              nextupdate; // When we will send updates to clients the next time
    uint64              oldticks;   // These here for tracking time
    uint64              newticks;   // These here for tracking time
    uint32              mPort;

    void processEvents(ap::net::NetData *pNetData);
    void updateObjects(float dt, ap::net::NetData* pNetData) const;
    void fireWeapons(uint64 tstamp, ap::net::NetData *pNetData);
    void weaponFired(ap::net::NetData *pNetData, ap::MovingObject *source);
    void detectCollisions(ap::net::NetData *pNetData) const;
    void relocateSpawnedMech(ap::Mech *mech) const;

    void createNewConnection(ap::uint32 userId, ap::net::NetData *netData);
}; // class Server

} // namespace server
} // namespape ap

#endif
