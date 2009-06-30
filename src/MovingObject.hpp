#ifndef AP_MOVING_OBJECT
#define AP_MOVING_OBJECT

#include <Ogre.h>

#include "NetObject.h"
#include "RectBoundaries.hpp"

namespace Ap {

class MovingObject : public NetObject {
    public:
    MovingObject(float nFriction = 0.05f, Ogre::Vector3 startingSpeed = Ogre::Vector3::ZERO);
    virtual ~MovingObject();

    void setWorldBoundaries(float top, float bottom, float left, float right);

    void setMaxSpeed(float speed);
    void setVelocity(Ogre::Vector3 velocity);
    Ogre::Vector3 getVelocity() const { return velocity; };

    void setPosition(Ogre::Vector3 pos) { location = pos; };

    void addForwardAcceleration(float amount);
    void addClockwiseTurningSpeed(float amountRad);

    void setForwardAcceleration(float amount) { accelerationFwd = amount; }
    void setClockwiseTurningSpeed(float amount) { velocityCWiseTurning = amount; }

    Ogre::Vector3 getFacing() const;

    Ogre::SceneNode* getOwnerNode() const { return pOwnerNode; }
    void setOwnerNode(Ogre::SceneNode *node) { pOwnerNode = node; }
    bool hasOwnerNode() const { return (!pOwnerNode); }

    void update(unsigned long msSinceLast);

    private:
    Ogre::Vector3       location;
    Ogre::Quaternion    orientation;
    Ogre::Vector3       initialFacing;
    Ogre::Vector3       velocity;
    float               accelerationFwd;
    float               velocityCWiseTurning;
    float               friction;

    RectBoundaries      worldBoundaries;
    float               maxSpeedSquared;

    Ogre::SceneNode     *pOwnerNode;

    void updateVelocity(unsigned long msSinceLast);
    void updateFacing(unsigned long msSinceLast);
    void updatePosition(unsigned long msSinceLast);
};

} // namespace Ap

#endif
