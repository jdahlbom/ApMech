#ifndef AP_MOVING_OBJECT
#define AP_MOVING_OBJECT

#include <Ogre.h>

#include "net/NetObject.h"
#include "net/Serializable.h"
#include "RectBoundaries.hpp"

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
};

class MovableControl : public net::Serializable {
    public:
    float               accelerationFwd;
    float               velocityCWiseTurning;

    MovableControl();
    ~MovableControl() { /* Do nothing */ }
    int serialize(uint8 *buffer, int start, int buflength) const;
    int unserialize(uint8 *buffer, int start);
};

class MovingObject : public net::NetObject {
    public:
    MovingObject(float nFriction = 0.05f, Ogre::Vector3 startingSpeed = Ogre::Vector3::ZERO);
    virtual ~MovingObject();

    void setWorldBoundaries(float top, float bottom, float left, float right);

    void setMaxSpeed(float speed);
    void setVelocity(Ogre::Vector3 velocity);
    Ogre::Vector3 getVelocity() const { return state->velocity; };

    void setPosition(Ogre::Vector3 pos) { state->location = pos; };

    void addForwardAcceleration(float amount);
    void addClockwiseTurningSpeed(float amountRad);

    void setForwardAcceleration(float amount) { control->accelerationFwd = amount; }
    void setClockwiseTurningSpeed(float amount) { control->velocityCWiseTurning = amount; }

    Ogre::Vector3 getFacing() const;

    Ogre::SceneNode* getOwnerNode() const { return pOwnerNode; }
    void setOwnerNode(Ogre::SceneNode *node) { pOwnerNode = node; }
    bool hasOwnerNode() const { return pOwnerNode; }
    void updateNode();

    void update(unsigned long msSinceLast);

    //NetObject serialization
    int serialize(uint8 *buffer, int start, int buflength) const;
    int unserialize(uint8 *buffer, int start);
    NetObject *create(int id);

    private:
    Ogre::Vector3       initialFacing;
    MovableState        *state;
    MovableControl      *control;
    float               friction;

    RectBoundaries      worldBoundaries;
    float               maxSpeedSquared;

    Ogre::SceneNode     *pOwnerNode;

    void updateVelocity(unsigned long msSinceLast);
    void updateFacing(unsigned long msSinceLast);
    void updatePosition(unsigned long msSinceLast);
};

} // namespace ap

#endif
