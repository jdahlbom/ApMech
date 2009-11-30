#ifndef AP_GAMEWORLD_H
#define AP_GAMEWORLD_H

#include "types.h"
#include "constants.h"
#include "RectBoundaries.h"
#include "MovingObject.h"
#include "net/NetObject.h"
#include "TerrainHeightMap.h"

#include <string>

namespace ap {

class GameWorld : public ap::net::NetObject
{
 public:
    std::string mapFileName, terrainFileName;
    RectBoundaries boundaries;

    GameWorld();
    GameWorld(std::string fname);
    ~GameWorld();

    void loadMapFile(const std::string fname);
    bool clampToWorldBoundaries(ap::MovingObject &movingObject);
	float getHeightAt(float x, float z);

    // NetObject interface
    net::NetObject *create(uint32 id) const;
    uint8 getObjectType() const;

    //NetObject serialization
    int serialize(uint8 *buffer, int start, int buflength) const;
    int unserialize(uint8 *buffer, int start);

 private:
    TerrainHeightMap *heightMap;
};

} // namespace ap

#endif
