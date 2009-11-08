#ifndef AP_SERVER_SERVER_HPP
#define AP_SERVER_SERVER_HPP

#include "../net/netdata.h"
#include "../net/NetObject.h"
#include "../net/NetUser.h"
#include "../net/NetEvent.h"

#include "../Mech.h"
#include "../MovingObject.h"
#include "../ScoreListing.h"
#include "../types.h"
#include "../GameWorld.h"

#include <set>

namespace ap {
namespace server {

class Server
{
    public:
    Server(uint32 port);
    ~Server();
    void start();

    private:
    ap::net::NetData    *netdata;
    ap::ScoreListing    *mScores;
    ap::GameWorld       *gameWorld;
    uint64              nextupdate; // When we will send updates to clients the next time
    uint64              oldticks;   // These here for tracking time
    uint64              newticks;   // These here for tracking time
    uint32              mPort;

    std::set<uint32>   pendingClients; // A new client is added here, and removed when first data comes from him.

    static const uint32        NetFPS = 20;

    void processEvents(ap::net::NetData *pNetData);
    void processPendingClients(ap::net::NetData *pNetData);
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
