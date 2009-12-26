#include "Mech.h"

#ifndef WIN32
#include <OgreVector3.h>
#else
#include <Ogre/OgreVector3.h>
#endif

#include "net/serializer.h"
#include "ObjectDataModel.h"

namespace ap {

  // Injected proto
class MechInject {
public:
  MechInject() {
    ap::net::netobjectprototypes().insert(
      std::make_pair(ap::OBJECT_TYPE_MECH, new Mech(Ogre::Vector3::ZERO, NULL)));
  }
};
static MechInject __mechinject;


Mech::Mech(Ogre::Vector3 velocity, ObjectDataModel *model) :
  MovingObject(0.0f, velocity, model, ap::OBJECT_TYPE_MECH),
  mIsIdle(true),
  mAnimState(0)
{
    objectType = ap::OBJECT_TYPE_MECH;
}

  void Mech::hookUpdate(float sSinceLast) {
      // if idle
    if (mIsIdle) {
    //   if started moving
      if (getVelocity().squaredLength() > 0.0f) {
	Ogre::Entity *ent = getEntity();
	assert( ent!=0);

	mAnimState = ent->getAnimationState("Walk");
	mAnimState->setLoop(true);
	mAnimState->setEnabled(true);
	mIsIdle = false;
      }
    } else {
      if (getVelocity().squaredLength() == 0.0f) {
	assert(mAnimState!=0);
	mAnimState->setEnabled(false);
	mIsIdle = true;
      }
    }

    if (!mIsIdle) {
      assert(mAnimState != 0);
      float distMovedDuringAnim = 18.0f;
      float durationOfAnim = 3.96f;
      float speedInAnim = distMovedDuringAnim / durationOfAnim;
      float realSpeed = getVelocity().length();

      mAnimState->addTime(realSpeed/speedInAnim * sSinceLast);
    }
  }

net::NetObject *Mech::create(uint32 _id) const
{
    Mech *mech = new Mech(Ogre::Vector3::ZERO, ObjectDataModel::getInstance());
    mech->id = _id;
    return mech;
}

uint8 Mech::getObjectType() const
{
    return ap::OBJECT_TYPE_MECH;
}


int Mech::serialize(uint8 *buffer, int start, int buflength) const
{
    using ap::net::serialize;

    int length = 0;
    length += MovingObject::serialize(buffer, start+length, buflength);
    length += serialize(color, buffer, start+length, buflength);
    return length;
}

int Mech::unserialize(uint8 *buffer, int start)
{
    using ap::net::unserialize;

    int length = 0;
    length += MovingObject::unserialize(buffer, start+length);
    length += unserialize(color, buffer, start+length);
    return length;
}

} // namespace ap
