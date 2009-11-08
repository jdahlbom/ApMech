#ifndef AP_GAMEWORLD_H
#define AP_GAMEWORLD_H

#include "types.h"
#include "constants.h"
#include "RectBoundaries.h"
#include "MovingObject.h"
#include "net/NetObject.h"
#include <string>

namespace ap {

class GameWorld : public ap::net::NetObject
{
 public:
    std::string mapFileName;
    RectBoundaries boundaries;

    GameWorld();
    GameWorld(std::string fname);
    ~GameWorld() {}

    void loadMapFile(std::string fname);
    bool clampToWorldBoundaries(ap::MovingObject &movingObject);

    // NetObject interface
    net::NetObject *create(uint32 id) const;
    uint8 getObjectType() const;

    //NetObject serialization
    int serialize(uint8 *buffer, int start, int buflength) const;
    int unserialize(uint8 *buffer, int start);
};

} // namespace ap

#endif
