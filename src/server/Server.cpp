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

namespace ap {
namespace server {

using namespace std;

Server::Server(unsigned int port) :
  mPort(port)
{
    std::cout << "[SERVER] Created a server."<< std::endl;
}

void Server::start() {
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
        oldticks = newticks;
        newticks = getTicks();
        float dt = static_cast<float>(newticks - oldticks) * 0.001;


        updateObjects(dt, netdata);
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
                << uint2ipv4(pNetData->users[event.uid].peer->address.host)
                <<", uid " << event.uid;
            cout << ". We now have "<< pNetData->users.size()<<" users."<<endl;

            int newid = pNetData->getUniqueObjectID();   // and an unused object id

            ap::MovingObject *newAvatar = new ap::Mech();
            newAvatar->setWorldBoundaries(1500.0f,0.0f,0.0f,1500.0f);
            newAvatar->setMaxSpeed(25.0f);
            // newAvatar->setLocation(Ogre::Vector3(XXX, YYY, ZZZ)
            pNetData->users[event.uid].setControlPtr(newAvatar->getControlPtr());
            pNetData->netobjects.insert(make_pair(newid, newAvatar));

            pNetData->setAvatarID(event.uid, newid);
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


void Server::updateObjects(uint64 dt, ap::net::NetData* pNetData) const {
    ap::net::NetData::netObjectsType::const_iterator objIterator;

    for( objIterator = pNetData->netobjects.begin(); objIterator!=pNetData->netobjects.end(); ++objIterator) {
        objIterator->second->advance(dt);
    }
} // void Server::updateObjects


void Server::detectCollisions(ap::net::NetData *pNetData) const {
    ap::net::NetData::netObjectsType::iterator mechIter, projIter;
    for(mechIter=pNetData->netobjects.begin(); mechIter!=pNetData->netobjects.end(); ++mechIter)
    {
        ap::Mech *mech = dynamic_cast<ap::Mech *>(mechIter->second);
        if (!mech)
            continue;

        for(projIter=pNetData->netobjects.begin(); projIter!=pNetData->netobjects.end(); ++projIter)
        {
            ap::Projectile *proj = dynamic_cast<ap::Projectile *>(projIter->second);
            if ((proj) && (proj->testCollision(*mech))) {
                relocateSpawnedMech(mech);
                netdata->delObject(projIter->second->id);  // This erases a pair from netobjects map,
            }                                           // (hope this doesn't do harm),
        }
    }
}

void Server::relocateSpawnedMech(ap::Mech *mech) const
{
    Ogre::Vector3 newPosition = Ogre::Vector3(rand()%1500, 0, rand()%1500);
    mech->setPosition(newPosition);
    mech->setVelocity(Ogre::Vector3::ZERO);
}

} // namespace server
} // namespace ap
