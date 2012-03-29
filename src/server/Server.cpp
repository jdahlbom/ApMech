#include "Server.h"

#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#ifndef WIN32
#include <OgreConfigFile.h>
#include <OgreVector3.h>
#else
#include <Ogre/OgreConfigFile.h>
#include <Ogre/OgreVector3.h>
#endif

#include <iostream>
#include <cstdlib>  // for random
#include <cmath>    // for sin,cos in the server
#include <list>

#include "../functions.h"
#include "../Mech.h"
#include "../Projectile.h"
#include "../MovingObject.h"
#include "../MechData.h"
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

    loadSettings("apserver.cfg");

    netdata->insertObject(mScores);
    netdata->insertObject(gameWorld);

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
            nextupdate = newticks + (1000/networkFPS); // 40 ms between updates would be 25 network fps.
            cout << "\rData rate: "<<changes<<"      ";
            cout.flush();
        }                               // Seems to me that up to 100 is still okay!

        ap::mSleep(1);       // sleep even a little bit so that dt is never 0

        processEvents(netdata);

        // Does matchRules say somebody is allowed to spawn now?
        while (int uidSpawner = matchRules.getSpawner()) usersWithoutAvatar.push_back(uidSpawner);
        if (!pendingClients.empty()) processPendingClients(netdata);
    } // Main loop
} // void Server::start()


    /** Load game-specific settings and mech data into apserver and initialize gameWorld */
bool Server::loadSettings(std::string serverConfigFile)
{
    // First, load the generic config parameters
    serverConfig.load(bundlePath() + serverConfigFile);
    if (!from_string<uint32>(networkFPS, serverConfig.getSetting("NetworkFPS"), std::dec)) networkFPS = 20;
    if ((gameRules = serverConfig.getSetting("DefaultGame")) == "") gameRules = "deathmatch";

    matchRules.load(gameRules, bundlePath() + serverConfigFile);

    if (!gameWorld) gameWorld = new ap::GameWorld();
    gameWorld->loadMapFile(serverConfig.getSetting("Map") + ".map");

    mechDB = new MechDatabase("data/mechs");    // the mech data files are in directory data/mechs
    if (mechDB == NULL) {
        std::cout << "[SERVER:loadSettings] Error accessing mech data files, exiting!" << std::endl;
        return false;
    }
    mechDB->readMechFiles();

    std::vector<std::string> mechNames = mechDB->getMechNames();
    std::vector<std::string>::const_iterator it;
    for (it=mechNames.begin(); it!=mechNames.end(); ++it) {
        std::cout << "Trying to create proto [" << *it << "]" << std::endl;
        MechData *data = mechDB->getMechData(*it);
	std::cout << "Creating proto [" << data->getName() << "]" << std::endl;
        netdata->insertObject(data);
    }
    return true;
}

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
        if (pProj->getPosition().y < gameWorld->getHeightAt(pProj->getPosition().x, pProj->getPosition().z))
            pProj->clamped = true;          // Then Xplode!
    }

    while (Mech *pMech = pNetData->eachObject<Mech *>(ap::OBJECT_TYPE_MECH)) {

        /* TODO: rotate the turret toward the mouse coordinates. The
         * coordinates are given in CombinedControls, the current turret
         * position is in pMech->getTorsoAngle(), and the maximum angle
         * that the turret may have can be found in MechData class
         * object method getTorsoMaxAngle(). */

        gameWorld->clampToWorldBoundaries(*pMech);
        Ogre::Vector3 mechLoc = pMech->getPosition();
        mechLoc.y = gameWorld->getHeightAt(mechLoc.x, mechLoc.z);
        pMech->setPosition(mechLoc);
        if (pMech->uid > 0) pMech->setChanged(); // Send living mechs always. For now. Everything else is predicted / as needed.
    }
} // void Server::updateObjects


void Server::fireWeapons(uint64 tstamp, ap::net::NetData *pNetData) {
    while (ap::Mech *mech = pNetData->eachObject<ap::Mech *>(ap::OBJECT_TYPE_MECH))
        if (mech->fireGun(tstamp)) weaponFired(pNetData, mech);
}

void Server::weaponFired(ap::net::NetData *pNetData, ap::MovingObject *source) {
    Ogre::Vector3 facing = source->getFacing();

    int newid = pNetData->insertObject(new ap::Projectile(facing * 40.0f)); //150 is velocity
    ap::Projectile *bullet = pNetData->getObject<ap::Projectile *>(newid);
    bullet->setMaxSpeed(625.0f);
    Ogre::Vector3 horizDisplacement = facing*5.0f;
    Ogre::Vector3 vertDisplacement = Ogre::Vector3(0.0f, 6.0f, 0.0f);
    bullet->setPosition(source->getPosition() + horizDisplacement + vertDisplacement);
    bullet->setFacing(facing);
    bullet->uid = source->uid;
    bullet->setChanged();
}

void Server::detectCollisions(ap::net::NetData *pNetData) {

    while (ap::Mech *mech = pNetData->eachObject<ap::Mech *>(ap::OBJECT_TYPE_MECH))
    {
        while (ap::Projectile *proj = pNetData->eachObject<ap::Projectile *>(ap::OBJECT_TYPE_PROJECTILE))
        {
            if (proj->testCollision(*mech) && (0 != mech->uid)) {
                matchRules.killPlayer(mScores, mech->uid, proj->uid);
                mScores->setChanged();
                mScores->print();

                pNetData->getUser(mech->uid)->setControlPtr(NULL);
                mech->getControlPtr()->reset();     // Dead girls don't say shoot
                mech->uid = 0;                      // The mech's not owned anymore

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
    newPlayer.clear();
    newPlayer.uid = userId;
    newPlayer.nick = pNetData->getUser(userId)->nick;
    mScores->addScore(newPlayer, true);
    mScores->setChanged();
    pNetData->alertObject(mScores->id);       // Refresh the score display to players
    gameWorld->setChanged();

//    std::vector<std::string> mechNames = mechDB->getMechNames(); // This ain't used

    // trigger the mech data to be sent to the new connection
    while (ap::MechData *mechData =
            pNetData->eachObject<ap::MechData *>(ap::OBJECT_TYPE_MECHDATA)) {
        mechData->setChanged();
    }

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
            // std::cout << "User has selected vehicle type " << pUser->chosenVehicleType << std::endl;
            Ogre::Vector3 initialSpeed(0,0,0);
            ap::Mech *newAvatar = new ap::Mech();
            int newid = pNetData->insertObject(newAvatar);
            ap::MechData *reader = mechDB->getMechData(pUser->chosenVehicleType);

            newAvatar->setTypeName(pUser->chosenVehicleType);
            if (reader) {
                newAvatar->setMaxTurnRate(reader->getTurnRate());
                newAvatar->setMaxForwardAcceleration(reader->getMaxForwardAcceleration());
                newAvatar->setMaxBackwardAcceleration(reader->getMaxBackwardAcceleration());
                newAvatar->setMaxSpeed(reader->getMaxSpeed());
            }
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
