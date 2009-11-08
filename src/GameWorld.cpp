#include "GameWorld.h"

#include "functions.h"
#include "net/serializer.h"

#ifndef WIN32
#include <OgreConfigFile.h>
#include <OgreVector3.h>
#else
#include <Ogre/OgreConfigFile.h>
#include <Ogre/OgreVector3.h>
#endif

#include <iostream>

//#include "net/netdata.h"

namespace ap {

/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class GameWorldInject {
/**/  public:
/**/    GameWorldInject() {
/**/      ap::net::netobjectprototypes().insert(
/**/        std::make_pair(ap::OBJECT_TYPE_GAMEWORLD, new GameWorld()));
/**/    }
/**/};
/**/static GameWorldInject __gameworldinject;


GameWorld::GameWorld() : boundaries(1.0, 0.0, 0.0, 1.0) {}

GameWorld::GameWorld(std::string fname) : boundaries(1.0, 0.0, 0.0, 1.0) {
    loadMapFile(mapFileName);
}

void GameWorld::loadMapFile(std::string fname) {
    mapFileName = fname;
    float top, bottom, left, right;

    bottom = left = 0.0;

    Ogre::ConfigFile mapConfig;
//    try {
    mapConfig.load(mapFileName);
    if (!from_string<float>(top, mapConfig.getSetting("PageWorldZ"), std::dec)) top = 1.0f;
    if (!from_string<float>(right, mapConfig.getSetting("PageWorldX"), std::dec)) right = 1.0f;
//    } catch (Ogre::FileNotFoundException e) {
//        top = right = 1.0;      // Bad values, but should be, as this is an error!
//    }

    boundaries.setBoundaries(top, bottom, left, right);
}

bool GameWorld::clampToWorldBoundaries(ap::MovingObject &movingObject) // state->location.z, state->location.x = horiz, vert
{
    Ogre::Vector3 loc = movingObject.getPosition();

    if( loc.z < boundaries.left ) {
        loc.z = boundaries.left;
        movingObject.clamped = true;
    } else if (loc.z > boundaries.right) {
        loc.z = boundaries.right;
        movingObject.clamped = true;
    }

    if ( loc.x < boundaries.bottom) {
        loc.x = boundaries.bottom;
        movingObject.clamped = true;
    } else if ( loc.x > boundaries.top) {
        loc.x = boundaries.top;
        movingObject.clamped = true;
    }

    if (movingObject.clamped) movingObject.setPosition(loc);
    return movingObject.clamped;
}

ap::net::NetObject* GameWorld::create(ap::uint32 _id) const {
    ap::net::NetObject *pNetObject = new GameWorld();
    pNetObject->id = _id;
    return pNetObject;
}

int GameWorld::serialize(uint8 buffer[], int start, int buflength) const
{
    int length = 0;
    length += ap::net::serialize(mapFileName, buffer, start+length, buflength-length);
    length += boundaries.serialize(buffer, start+length, buflength-length);
    return length;
}

int GameWorld::unserialize(uint8 buffer[], int start)
{
    int length = 0;
    length += ap::net::unserialize(mapFileName, buffer, start+length);
    length += boundaries.unserialize(buffer, start+length);
    return length;
}

ap::uint8 GameWorld::getObjectType() const {
    return ap::OBJECT_TYPE_GAMEWORLD;
}



} // namespace ap
