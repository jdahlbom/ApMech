#ifndef AP_MOVING_OBJECT
#define AP_MOVING_OBJECT

#include <Ogre.h>

#include "net/NetObject.h"
#include "net/Serializable.h"
#include "RectBoundaries.hpp"
#include "types.h"

// For MovableControl
#include "net/Controller.h"
#include "MovableControl.h"
#include "WeaponControl.h"
#include "CombinedControls.h"

namespace ap {

using namespace ap::net;

class MovingObject : public net::NetObject {
 public:
    MovingObject(float nFriction = 0.05f, Ogre::Vector3 startingSpeed = Ogre::Vector3::ZERO);
    virtual ~MovingObject();

    void setWorldBoundaries(float top, float bottom, float left, float right);
    bool hitWorldBoundaries() const;

    void setMaxSpeed(float speed);
    void setVelocity(Ogre::Vector3 velocity);
    Ogre::Vector3 getVelocity() const { return state->velocity; }

    void setPosition(Ogre::Vector3 pos) { state->location = pos; }
    Ogre::Vector3 getPosition() const { return state->location; }

    void addForwardAcceleration(float amount);
    void addClockwiseTurningSpeed(float amountRad);

    void setForwardAcceleration(float amount) { control->accelerationFwd = amount; }
    void setClockwiseTurningSpeed(float amount) { control->velocityCWiseTurning = amount; }

    Ogre::Vector3 getFacing() const;
    void setFacing(Ogre::Vector3 f);
    void setFriction(float f) { friction = f; }

    Ogre::SceneNode* getOwnerNode() const { return pOwnerNode; }
    void setOwnerNode(Ogre::SceneNode *node) { pOwnerNode = node; }
    bool hasOwnerNode() const { return pOwnerNode; }
    void updateNode();

    Controller* getControlPtr() const { return combinedControls; }
    MovableState* getStatePtr() const {return state; }

    void update(float sSinceLast);
    int advance(float dt) { update(dt); return 0; }

    // WeaponControl methods
    void setFiring(bool firingStatus);
    bool fireGun(uint64 tstamp);

    //NetObject serialization
    int serialize(uint8 *buffer, int start, int buflength) const;
    int unserialize(uint8 *buffer, int start);
    NetObject *create(uint32 id);

    bool testCollision(const MovingObject &other) const;

 protected:
    uint8              objectType;

 private:
    Ogre::Vector3       initialFacing;
    MovableState        *state;
    MovableControl      *control;

    WeaponControl       *weaponControl;
    CombinedControls    *combinedControls;

    float               friction;
    float               maxSpeedSquared;

    Ogre::SceneNode     *pOwnerNode;
    RectBoundaries      worldBoundaries;

    void updateVelocity(float sSinceLast);
    void updateFacing(float sSinceLast);
    void updatePosition(float sSinceLast);
};

} // namespace ap

#endif
