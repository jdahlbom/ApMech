#include "Server.hpp"

#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <iostream>
#include <cstdlib>  // for random
#include <cmath>    // for sin,cos in the server
#include <list>

#include "../functions.h"
#include "../Mech.h"
#include "../Projectile.h"
#include "../MovingObject.hpp"
#include "../types.h"

namespace ap {
namespace server {

using namespace std;

Server::Server(uint32 port) :
  mPort(port)
{
    std::cout << "[SERVER] Created a server."<< std::endl;
}

void Server::start() {
    float dt; // delta-t between main loop iterations
    std::cout << "[SERVER] Started the server."<<std::endl;
    netdata = new net::NetData(net::NetData::SERVER, mPort);

    if( !netdata ) {
        std::cout << "[SERVER] Error initializing NetData, exiting!" << std::endl;
        return;
    }

    std::cout << "[SERVER] Serving "<< ap::net::netobjectprototypes().size()<<" types of objects."<<std::endl;

    newticks = nextupdate = getTicks();

    while (1) {             // Server main loop
        netdata->receiveChanges();
        oldticks = newticks; newticks = getTicks();
        dt = float(newticks - oldticks) * 0.001;
//        ap::uint64 dt = newticks - oldticks;


        updateObjects(dt, netdata);
        fireWeapons(newticks, netdata);
        detectCollisions(netdata);


        if (newticks >= nextupdate) {
            netdata->sendChanges();
            nextupdate = newticks + 40; // 40 ms between updates, that's 25 network fps.
        }                               // Seems to me that up to 100 is still okay!
        // TODO: Ensure that usleep is available on Mac/Windows as well!
        usleep(1000);       // sleep even a little bit so that dt is never 0

        processEvents(netdata);
    } // Main loop
} // void Server::start()

void Server::processEvents(ap::net::NetData *pNetData) {
    ap::net::NetEvent event;
    while (pNetData->pollEvent(event))
    {
      switch (event.type)
      {
        case ap::net::NetData::EVENT_CONNECT:
        {
            cout << "[SERVER] Received a connection from "
                << uint2ipv4(pNetData->getUser(event.uid)->peer->address.host)
                <<", uid " << event.uid;
	    createNewConnection(event.uid, pNetData);
            break;
        }
        case ap::net::NetData::EVENT_DISCONNECT:
            cout << "[SERVER] Client "<<event.uid<<" disconnected."<<endl;
            break;
        default:
            break;
        }
    }
} // void Server::processEvents(netdata*)


void Server::updateObjects(float dt, ap::net::NetData* pNetData) const {

    while (NetObject *nop = pNetData->eachObject()) nop->advance(dt);

} // void Server::updateObjects

void Server::fireWeapons(uint64 tstamp, ap::net::NetData *pNetData) {
    while (ap::Mech *mech = pNetData->eachObject<ap::Mech *>(ap::OBJECT_TYPE_MECH))
        if (mech->fireGun(tstamp)) weaponFired(pNetData, mech);
}

void Server::weaponFired(ap::net::NetData *pNetData, ap::MovingObject *source) {
    Ogre::Vector3 facing = source->getFacing();

    int newid = pNetData->insertObject(new ap::Projectile(facing * 100.0f)); //100 is velocity
    ap::Projectile *bullet = pNetData->getObject<ap::Projectile *>(newid);
    bullet->setWorldBoundaries(1500.0f,0.0f,0.0f,1500.0f);
    bullet->setMaxSpeed(625.0f);
    bullet->setPosition(source->getPosition() + facing*10.0f + Ogre::Vector3(0.0f, 80.0f, 0.0f));
    bullet->setFacing(facing);
}

void Server::detectCollisions(ap::net::NetData *pNetData) const {

    while (ap::Mech *mech = pNetData->eachObject<ap::Mech *>(ap::OBJECT_TYPE_MECH))
        while (ap::Projectile *proj = pNetData->eachObject<ap::Projectile *>(ap::OBJECT_TYPE_PROJECTILE))
            if (proj->testCollision(*mech)) {
                relocateSpawnedMech(mech);
                netdata->delObject(proj->id);
            }
}

void Server::relocateSpawnedMech(ap::Mech *mech) const
{
    Ogre::Vector3 newPosition = Ogre::Vector3(rand()%1500, 0, rand()%1500);
    mech->setPosition(newPosition);
    mech->setVelocity(Ogre::Vector3::ZERO);
}

void Server::createNewConnection(ap::uint32 userId, ap::net::NetData *netData)
{
  ap::Mech *newAvatar = new ap::Mech();
  int newid = netData->insertObject(newAvatar);

  newAvatar->setWorldBoundaries(1500.0f,0.0f,0.0f,1500.0f);
  newAvatar->setMaxSpeed(35.0f);
  newAvatar->setFriction(8.0f);
  relocateSpawnedMech(newAvatar);

  netData->getUser(userId)->setControlPtr(newAvatar->getControlPtr());
  
  netData->sendChanges();
  netData->setAvatarID(userId, newid);
}

} // namespace server
} // namespace ap
