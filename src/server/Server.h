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
#include "../ObjectDataBase.h"

#include <set>
#include <list>
#include <map>

namespace ap {
namespace server {

class Server
{
 public:
    Server(uint32 port);
    ~Server();
    void start();

 private:

    Ogre::ConfigFile serverConfig;

    ap::net::NetData    *netdata;
    ap::ScoreListing    *mScores;
    ap::GameWorld       *gameWorld;
    ap::MechDatabase    *mechDB;
    uint64              nextupdate; // When we will send updates to clients the next time
    uint64              oldticks;   // These here for tracking time
    uint64              newticks;   // These here for tracking time
    uint32              mPort;

    std::set<uint32>        pendingClients; // A new client is added here, and removed when first data comes from him.
    std::list<ap::uint32>   usersWithoutAvatar;

    std::map<ap::uint32, ap::uint32>    deadClients; // ticks value when respawn, uid whom to respawn

// Server settings
    uint32              networkFPS;     // How many times per second we try to send updates to clients
    uint32              respawnDelay;   // How many milliseconds players stay dead before re-entering the game
    std::string         gameRules;      // What rules to use in the game. Deathmatch currently only implemented set of rules.
// end

    void processEvents(ap::net::NetData *pNetData);
    void processPendingClients(ap::net::NetData *pNetData);
    void processDeadClients(ap::net::NetData *pNetData);
    void updateObjects(float dt, ap::net::NetData* pNetData) const;
    void fireWeapons(uint64 tstamp, ap::net::NetData *pNetData);
    void weaponFired(ap::net::NetData *pNetData, ap::MovingObject *source);
    void detectCollisions(ap::net::NetData *pNetData); // not const anymore. Needs to change deadClients
    void relocateSpawnedMech(ap::Mech *mech) const;
    void spawnNewAvatars(ap::net::NetData *pNetData);

    void createNewConnection(ap::uint32 userId, ap::net::NetData *netData);
}; // class Server

} // namespace server
} // namespape ap

#endif
