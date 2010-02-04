#ifndef AP_MOVING_OBJECT
#define AP_MOVING_OBJECT

#include <string>
#include <map>

#ifndef WIN32
#include <Ogre.h>
#else
#include <Ogre/Ogre.h>
#endif

#include "net/NetObject.h"
#include "net/Serializable.h"
#include "RectBoundaries.h"
#include "types.h"

// For MovableControl
#include "net/Controller.h"
#include "MovableControl.h"
#include "WeaponControl.h"
#include "TurretControl.h"
#include "CombinedControls.h"

namespace ap {

using namespace ap::net;

class MovingObject : public net::NetObject {
 public:
    bool clamped;   // Whether this object's location has been clamped to world boundaries

    MovingObject(float nFriction = 0.05f, Ogre::Vector3 startingSpeed = Ogre::Vector3::ZERO, uint8 type_id = 0);
    virtual ~MovingObject();

    void setMaxSpeed(float speed);
    void setVelocity(Ogre::Vector3 velocity);
    Ogre::Vector3 getVelocity() const { return state->velocity; }

    void setPosition(Ogre::Vector3 pos) { state->location = pos; }
    Ogre::Vector3 getPosition() const { return state->location; }

    void setMaxForwardAcceleration(float amount);
    void setMaxBackwardAcceleration(float amount);
    void setMaxTurnRate(float amount);

    void addForwardAcceleration(float amount);
    void addClockwiseTurningSpeed(float amountRad);

    void setForwardAcceleration(float amount);
    void setClockwiseTurningSpeed(float amount);

    Ogre::Vector3 getFacing() const;
    void setFacing(Ogre::Vector3 f);
    void setFriction(float f) { friction = f; }

    Ogre::SceneNode *getOwnerNode() const { return pOwnerNode; }
    void setOwnerNode(Ogre::SceneNode *node) { pOwnerNode = node; }
    bool hasOwnerNode() const { return pOwnerNode; }
    void updateNode();

    Controller* getControlPtr() const { return combinedControls; }
    MovableState* getStatePtr() const {return state; }

    void update(float sSinceLast);

    bool testCollision(const MovingObject &other) const;

    // Update-callback for inheritors. TODO: Refactor MovingObject! 
    virtual void hookUpdate(float sSinceLast)=0;

    // WeaponControl methods
    void setFiring(bool firingStatus);
    bool fireGun(uint64 tstamp);

    // TurretControl methods
    void setAimCoordinates(Ogre::Vector3 target);

    //NetObject serialization
    int serialize(uint8 *buffer, int start, int buflength) const;
    int unserialize(uint8 *buffer, int start);
    ap::net::NetObject *create(uint32 id) { return 0; }; // FIXME: Evil!
    int advance(float dt);
    virtual uint8 getObjectType() const = 0;

    void addEntity(std::string name, Ogre::SceneNode* node, Ogre::Entity* entity);

 protected:
    uint8              objectType;
    float               friction;
    float               maxSpeedSquared;
    bool                mIsClient;
    std::map<std::string, Ogre::SceneNode*> pMeshNodes;
    std::map<std::string, Ogre::Entity*>    pEntities;

 private:
    Ogre::Vector3       initialFacing;
    MovableState        *state;
    MovableControl      *control;

    WeaponControl       *weaponControl;
    TurretControl       *turretControl;
    CombinedControls    *combinedControls;

    Ogre::SceneNode             *pOwnerNode;

    void updateVelocity(float sSinceLast);
    void updateFacing(float sSinceLast);
    void updatePosition(float sSinceLast);

    float maxForwardAcceleration;
    float maxBackwardAcceleration;
    float turningRate;
};

} // namespace ap

#endif
