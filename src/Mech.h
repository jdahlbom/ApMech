#ifndef AP_MECH_H
#define AP_MECH_H

#include "MovingObject.h"
#include "types.h"
#include "constants.h"

#ifndef WIN32
#include <OgreVector3.h>
#else
#include <Ogre/OgreVector3.h>
#endif

namespace ap {

class Mech : public MovingObject
{
 public:
    uint32 color;

    Mech(Ogre::Vector3 velocity = Ogre::Vector3::ZERO);
    ~Mech() {}

    // Overridden MovingObject methods
    void hookUpdate(float sSinceLast);

    // NetObject interface .. that was not implemented by MovingObject
    net::NetObject *create(uint32 id) const;
    uint8 getObjectType() const;
    std::string getTypeName() const;
    void setTypeName(std::string);
    int getTorsoAngle();
    void setTorsoAngle(int angle);

    //NetObject serialization
    int serialize(uint8 *buffer, int start, int buflength) const;
    int unserialize(uint8 *buffer, int start);

 private:
    bool                 mIsIdle;
    Ogre::AnimationState *mAnimState;
    std::string          typeName;
    int torsoAngle;
};

} // namespace ap

#endif
