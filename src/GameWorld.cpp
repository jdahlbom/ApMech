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

GameWorld::~GameWorld() {
    if (heightMap) delete heightMap;
}

void GameWorld::loadMapFile(std::string fname) {
    mapFileName = fname;
    float top, bottom, left, right, tmp, maxHeight;

    bottom = left = 0.0;

    //    try { // FIXME: Should we try-catch or not? It's all well that all goes fubar if files are wrong/missing
    Ogre::ConfigFile mapConfig, terrainConfig;
    mapConfig.load(bundlePath() + "data/maps/" + mapFileName);
    terrainFileName = "data/maps/" + mapConfig.getSetting("TerrainFile");
    terrainConfig.load(bundlePath() + terrainFileName);

    if (!from_string<float>(top, terrainConfig.getSetting("PageWorldZ"), std::dec)) top = 1.0f;
    if (!from_string<float>(right, terrainConfig.getSetting("PageWorldX"), std::dec)) right = 1.0f;
    if (!from_string<float>(maxHeight, terrainConfig.getSetting("MaxHeight"), std::dec)) maxHeight = 256.0f;

    std::string heightMapFile = "Media/materials/textures/" + terrainConfig.getSetting("Heightmap.image");
    heightMap = new TerrainHeightMap(0, right, maxHeight, heightMapFile, false);    // FIXME: If level is not square, this fails silently. BAD.

    if (from_string<float>(tmp, mapConfig.getSetting("MapBorderTop"), std::dec)) top = tmp;
    if (from_string<float>(tmp, mapConfig.getSetting("MapBorderBottom"), std::dec)) bottom = tmp;
    if (from_string<float>(tmp, mapConfig.getSetting("MapBorderLeft"), std::dec)) left = tmp;
    if (from_string<float>(tmp, mapConfig.getSetting("MapBorderRight"), std::dec)) right = tmp;

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

float GameWorld::getHeightAt(float x, float z)
{
    if (heightMap) return heightMap->getHeightAt(x, z);
    else return 0.0f;
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
    length += ap::net::serialize(terrainFileName, buffer, start+length, buflength-length);
    length += boundaries.serialize(buffer, start+length, buflength-length);
    return length;
}

int GameWorld::unserialize(uint8 buffer[], int start)
{
    int length = 0;
    length += ap::net::unserialize(mapFileName, buffer, start+length);
    length += ap::net::unserialize(terrainFileName, buffer, start+length);
    length += boundaries.unserialize(buffer, start+length);
    return length;
}

ap::uint8 GameWorld::getObjectType() const {
    return ap::OBJECT_TYPE_GAMEWORLD;
}



} // namespace ap
