#include "MovableControl.h"

#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreMath.h>
#include <OgrePrerequisites.h>

#include "types.h"
#include "net/serializer.hpp"

namespace ap {

// MovableState ---------------------------------------------------------------
MovableState::MovableState(Ogre::Vector3 startingVelocity):
    location(Ogre::Vector3::ZERO),
    orientation(Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z),
    velocity(startingVelocity) {}

int MovableState::serialize(uint8 *buffer, int start, int buflength) const {
    int length = 0;
    length += net::serialize(location, buffer, start+length, buflength-length);
    length += net::serialize(orientation, buffer, start+length, buflength-length);
    length += net::serialize(velocity, buffer, start+length, buflength-length);
    return length;
}

int MovableState::unserialize(uint8 *buffer, int start) {
    int length = 0;
    length += net::unserialize(location, buffer, start+length);
    length += net::unserialize(orientation, buffer, start+length);
    length += net::unserialize(velocity, buffer, start+length);
    return length;
}

bool MovableState::testCollision(const MovableState &other) const {
    // TODO: Distance is a inferior solution, func should be replaced with real collision detection
    Ogre::Real sqDistance = location.squaredDistance(other.location);

    if (sqDistance < 10.0f) {
        return true;
    }
    return false;
}

void MovableState::dump() {
    std::cout << "[MOVABLESTATE:dump] location: " << location.x <<"|"<<location.y<<"|"<<location.z
                                << ", velocity: " << velocity.x<<"|"<<velocity.y<<"|"<<velocity.z
                                << std::endl;
}

// MovableControl--------------------------------------------------------------
MovableControl::MovableControl():
    accelerationFwd(0),
    velocityCWiseTurning(0) {}

int MovableControl::serialize(uint8 *buffer, int start, int buflength) const {
    int length = 0;
    length += net::serialize(accelerationFwd, buffer, start+length, buflength-length);
    length += net::serialize(velocityCWiseTurning, buffer, start+length, buflength-length);
    return length;
}

int MovableControl::unserialize(uint8 *buffer, int start) {
    int length = 0;
    length += net::unserialize(accelerationFwd, buffer, start+length);
    length += net::unserialize(velocityCWiseTurning, buffer, start+length);
    return length;
}

void MovableControl::dump() {
    std::cout << "[MOVABLECONTROL::dump] Acceleration: " << accelerationFwd
                                    << "Turning: " << velocityCWiseTurning << std::endl;
}

} // namespace ap