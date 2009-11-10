#include "Server.h"

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
#include "../MovingObject.h"
#include "../types.h"
#include "../net/NetMessage.h"

namespace ap {
namespace server {
using namespace std;



Server::Server(uint32 port) :
  mScores(new ap::ScoreListing()),
  mPort(port),
  usersWithoutAvatar(std::list<ap::uint32>())
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

    netdata->insertObject(mScores);
    netdata->insertObject(gameWorld = new ap::GameWorld());
    gameWorld->loadMapFile("data/maps/glacier.map");

    newticks = nextupdate = getTicks();

    while (1) {             // Server main loop
        netdata->receiveChanges();
        oldticks = newticks; newticks = getTicks();
        dt = float(newticks - oldticks)*0.001; // dt is in seconds

        updateObjects(dt, netdata);
        fireWeapons(newticks, netdata);
        detectCollisions(netdata);
        spawnNewAvatars(netdata);

        if (newticks >= nextupdate) {
            int changes = netdata->sendChanges();
            nextupdate = newticks + (1000/NetFPS); // 40 ms between updates, that's 25 network fps.
            cout << "\rData rate: "<<changes<<"      ";
            cout.flush();
        }                               // Seems to me that up to 100 is still okay!
        // TODO: Ensure that usleep is available on Mac/Windows as well!
        ap::mSleep(1);       // sleep even a little bit so that dt is never 0

        processEvents(netdata);
        if (!pendingClients.empty()) processPendingClients(netdata);
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
            pendingClients.insert(event.uid);   // Remember him until later!
                                                // If we add now, his NetUser is still uninitialized.
            break;
        }
        case ap::net::NetData::EVENT_DISCONNECT:
        {
            cout << "[SERVER] Client "<<event.uid<<" disconnected."<<endl;
            while (Mech *pM = pNetData->eachObject<Mech *>(ap::OBJECT_TYPE_MECH)) {
                if (pM->uid == event.uid) pNetData->delObject(pM->id);
            }
            mScores->removeScore(event.uid);
            pNetData->alertObject(mScores->id);       // Refresh the score display to players
            NetMessage disconnectMessage("!!! "+pNetData->getPastUser(event.uid)->nick+" has disconnected !!!");
            pNetData->sendMessage(disconnectMessage);
            break;
        }
        default:
            break;
        }
    }
} // void Server::processEvents(netdata*)

  /**
   * Make sure multiple connection attempts from one client do
   * not break the server by leaving hanging pending clients.
   */
void Server::processPendingClients(ap::net::NetData *pNetData) {
    std::set<uint32>::iterator i = pendingClients.begin();

    while (i != pendingClients.end()) {
        if (!pNetData->getUser(*i)) {
            pendingClients.erase(i++);      // The user disconnected before finishing connection
        } else if (pNetData->getUser(*i)->initialized) {
            createNewConnection(*i, pNetData);
            NetMessage connectMessage("!!! "+pNetData->getUser(*i)->nick+" has joined the game !!!");
            pNetData->sendMessage(connectMessage);
            pendingClients.erase(i++);
        } else i++;
    }
}

void Server::updateObjects(float dt, ap::net::NetData* pNetData) const {

    while (NetObject *nop = pNetData->eachObject()) {
        if (nop->advance(dt) == -1) pNetData->delObject(nop->id);
    }

    while (Projectile *pProj = pNetData->eachObject<Projectile *>(ap::OBJECT_TYPE_PROJECTILE)) {
        gameWorld->clampToWorldBoundaries(*pProj);
    }

    while (Mech *pMech = pNetData->eachObject<Mech *>(ap::OBJECT_TYPE_MECH)) {
        gameWorld->clampToWorldBoundaries(*pMech);
        pMech->setChanged();                        // Send mechs always. For now. Everything else is predicted / as needed.
    }
} // void Server::updateObjects


void Server::fireWeapons(uint64 tstamp, ap::net::NetData *pNetData) {
    while (ap::Mech *mech = pNetData->eachObject<ap::Mech *>(ap::OBJECT_TYPE_MECH))
        if (mech->fireGun(tstamp)) weaponFired(pNetData, mech);
}

  void Server::weaponFired(ap::net::NetData *pNetData, ap::MovingObject *source) {
    Ogre::Vector3 facing = source->getFacing();

    int newid = pNetData->insertObject(new ap::Projectile(facing * 150.0f)); //150 is velocity
    ap::Projectile *bullet = pNetData->getObject<ap::Projectile *>(newid);
    bullet->setMaxSpeed(625.0f);
    bullet->setPosition(source->getPosition() + facing*70.0f + Ogre::Vector3(0.0f, 80.0f, 0.0f));
    bullet->setFacing(facing);
    bullet->uid = source->uid;
    bullet->setChanged();
}

void Server::detectCollisions(ap::net::NetData *pNetData) const {

    while (ap::Mech *mech = pNetData->eachObject<ap::Mech *>(ap::OBJECT_TYPE_MECH))
      {
        while (ap::Projectile *proj = pNetData->eachObject<ap::Projectile *>(ap::OBJECT_TYPE_PROJECTILE))
	  {
            if (proj->testCollision(*mech)) {
                relocateSpawnedMech(mech);

                if (proj->uid == mech->uid) {   // He killed himself!
                    ap::ScoreTuple selfKiller;
                    selfKiller.uid = proj->uid;
                    selfKiller.kills = 0;
                    selfKiller.deaths = 1;
                    selfKiller.score = -1;
                    mScores->addScore(selfKiller, false);
                } else {                        // update scores normally.
                    ap::ScoreTuple projOwner;
                    projOwner.uid = proj->uid;
                    projOwner.kills = 1;
                    projOwner.deaths = 0;
                    projOwner.score = 1;
                    ap::ScoreTuple mechOwner;
                    mechOwner.uid = mech->uid;
                    mechOwner.kills = 0;
                    mechOwner.deaths = 1;
                    mechOwner.score = -1;
                    mScores->addScore(projOwner, false);
                    mScores->addScore(mechOwner, false);
                }

                mScores->setChanged();
                mScores->print();

                pNetData->delObject(proj->id);
                pNetData->alertObject(mScores->id);       // Refresh the score display to players
            }
	  }
      }
}

void Server::relocateSpawnedMech(ap::Mech *mech) const
{
    Ogre::Vector3 newPosition = Ogre::Vector3(rand() % int(gameWorld->boundaries.right - gameWorld->boundaries.left) + gameWorld->boundaries.left,
                                            0, rand() % int(gameWorld->boundaries.top - gameWorld->boundaries.bottom) + gameWorld->boundaries.bottom);
    mech->setPosition(newPosition);
    mech->setVelocity(Ogre::Vector3::ZERO);
}

void Server::createNewConnection(ap::uint32 userId, ap::net::NetData *pNetData)
{
  // Only create a new avatar if client has chosen the vehicle type.
  NetUser *newUser = pNetData->getUser(userId);
  assert(newUser != NULL);

  // Add scores:
  ap::ScoreTuple newPlayer;
  newPlayer.uid = userId;
  newPlayer.nick = pNetData->getUser(userId)->nick;
  newPlayer.kills = 0;
  newPlayer.deaths = 0;
  newPlayer.score = 0;
  mScores->addScore(newPlayer, true);
  mScores->setChanged();
  pNetData->alertObject(mScores->id);       // Refresh the score display to players
  gameWorld->setChanged();

  usersWithoutAvatar.push_back(userId);
  pNetData->sendChanges();
}

void Server::spawnNewAvatars(ap::net::NetData *pNetData) {
  std::list<ap::uint32>::iterator it = usersWithoutAvatar.begin();
  while(it != usersWithoutAvatar.end()) {
    ap::uint32 userId = *it;
    ap::net::NetUser *pUser = pNetData->getUser(userId);

    if (NULL == pUser) {
      it = usersWithoutAvatar.erase(it);
      continue;
    }

    if ("" != pUser->chosenVehicleType) {
      ap::Mech *newAvatar = new ap::Mech();
      int newid = pNetData->insertObject(newAvatar);

      newAvatar->setMaxSpeed(35.0f);
      newAvatar->setFriction(8.0f);
      newAvatar->uid = userId;
      newAvatar->color = pUser->color;
      relocateSpawnedMech(newAvatar);
      pNetData->alertObject(newAvatar->id); // Alert clients to paint this mech!

      pUser->setControlPtr(newAvatar->getControlPtr());

      it = usersWithoutAvatar.erase(it);
      pNetData->sendChanges(); // Shouldn't this occur in the main loop??

      pNetData->setAvatarID(userId, newid);
    } else {
      ++it;
    }
  }
}

Server::~Server() {
    if (mScores) delete mScores;
}

} // namespace server
} // namespace ap
