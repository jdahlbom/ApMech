#ifndef AP_MOVABLE_CONTROL_H
#define AP_MOVABLE_CONTROL_H

#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreMath.h>

#include "net/Serializable.h"
#include "Controller.h"
#include "types.h"

namespace ap {
class MovableState : public net::Serializable {
    public:
    Ogre::Vector3       location;
    Ogre::Quaternion    orientation;
    Ogre::Vector3       velocity;
    Ogre::Radian        ccwRotation;

    MovableState(Ogre::Vector3  startingVelocity);
    ~MovableState() { /* Do nothing */ }
    int serialize(uint8 *buffer, int start, int buflength) const;
    int unserialize(uint8 *buffer, int start);
    bool testCollision(const MovableState &other) const;
    void dump();
};

class MovableControl : public Controller {
    public:
    float               accelerationFwd;
    float               velocityCWiseTurning;

    MovableControl();
    ~MovableControl() { /* Do nothing */ }
    int serialize(uint8 *buffer, int start, int buflength) const;
    int unserialize(uint8 *buffer, int start);
    void dump();

};
} // namespace ap

#endif