#include "MovingObject.hpp"

#include <assert.h>
#include <Ogre.h>

#include "net/serializer.h"
#include "math/rotations.hpp"
#include "types.h"
#include "constants.h"

#include "MovableControl.h"
#include "WeaponControl.h"
#include "CombinedControls.h"

namespace ap {

MovingObject::MovingObject(float nFriction, Ogre::Vector3 startingVelocity):
    objectType(ap::OBJECT_TYPE_UNDEFINED),
    initialFacing(-Ogre::Vector3::UNIT_Z),
    state(new MovableState(startingVelocity)),
    control(new MovableControl()),
    weaponControl(new WeaponControl()),
    combinedControls(new CombinedControls(control, weaponControl)),
    friction(nFriction),
    worldBoundaries(0.0f, 0.0f, 0.0f, 0.0f),
    maxSpeedSquared(625.0),
    pOwnerNode(0)
{
    id = 0;
    uid = 0;
}

MovingObject::~MovingObject()
{
    delete(state);
    delete(control);
}

void MovingObject::setWorldBoundaries(float top, float bottom, float left, float right)
{
    worldBoundaries = RectBoundaries(top, bottom, left, right);
}

bool MovingObject::hitWorldBoundaries() const
{
    return worldBoundaries.clamped;
}

void MovingObject::setMaxSpeed(float speed)
{
    if(speed < 0) return;
    maxSpeedSquared = speed * speed;
}

/**
 * Asettaa nopeuden, tarkoitus käyttää alkunopeuden asettamiseksi.
 *
 * TODO: Tämän tulisi olla vain palvelimen tehtävissä.
 *
 * Kommentti: miksi? Jos client haksotetaan tekemään näin, se ainoastaan
 * aiheuttaa sen että client näkee asiat väärin, ei muuta. -Jonni
 */
void MovingObject::setVelocity(Ogre::Vector3 newVelocity)
{
    state->velocity = newVelocity;
}

void MovingObject::addForwardAcceleration(float amount)
{
    float maxAbsAcceleration = 70;  // was 25
    control->accelerationFwd += amount;

    if(control->accelerationFwd > maxAbsAcceleration) {
        control->accelerationFwd = maxAbsAcceleration;
    } else if (control->accelerationFwd < (-maxAbsAcceleration)) {
        control->accelerationFwd = 0-maxAbsAcceleration;
    }
}

void MovingObject::addClockwiseTurningSpeed(float amount)
{
    control->velocityCWiseTurning += amount;
}

void MovingObject::setFiring(bool firingStatus) {
    weaponControl->setFiring(firingStatus);
}

bool MovingObject::fireGun(uint64 tstamp) {
    return weaponControl->fire(tstamp);
}


/**
 * Returns the direction this object is facing.
 *
 * See below for detailed explanation:
 * http://www.ogre3d.org/wiki/index.php/Quaternion_and_Rotation_Primer#Resetting_Orientation
 *
 * return   Ogre::Vector3
 */
Ogre::Vector3 MovingObject::getFacing() const { return state->orientation * initialFacing; }

void MovingObject::setFacing(Ogre::Vector3 f)
{
   Ogre::Vector3 axes[3];
   state->orientation.ToAxes(axes);
   state->orientation = axes[0].getRotationTo(f);
}


void MovingObject::update(float sSinceLast)
{
    updateFacing(sSinceLast);
    updateVelocity(sSinceLast);
    updatePosition(sSinceLast);
}

void MovingObject::updateNode()
{
    if(pOwnerNode) {
        pOwnerNode->setPosition(state->location);
        pOwnerNode->setOrientation(state->orientation);
    } else {
        std::cout << "[MOVINGOBJECT:updateNode] No pOwnerNode!" << std::endl;
    }
}

void MovingObject::updateVelocity(float sSinceLast)
{
    state->velocity += control->accelerationFwd * sSinceLast * getFacing();
    Ogre::Real length = state->velocity.length();

    if ((length < friction*sSinceLast) || (length == 0.0f))
        state->velocity = Ogre::Vector3::ZERO;
    else {
        Ogre::Vector3 unitVelocity = state->velocity / length;
        state->velocity = state->velocity - unitVelocity*(friction * sSinceLast);

        Ogre::Real squaredLength = state->velocity.squaredLength();
        if ((squaredLength > maxSpeedSquared) && (squaredLength > 0.0f))
            state->velocity = (state->velocity / squaredLength) * maxSpeedSquared;
    }
}

void MovingObject::updateFacing(float sSinceLast)
{
    if(control->velocityCWiseTurning != 0) {
        Ogre::Radian turning = Ogre::Radian(control->velocityCWiseTurning * sSinceLast);
        state->ccwRotation += turning;
        Ogre::Matrix3 rotMatrix(ap::math::getRotationMatrixAfterYaw(state->ccwRotation));
        state->orientation = Ogre::Quaternion(rotMatrix);
    }
}

void MovingObject::updatePosition(float sSinceLast)
{
    state->location = state->location + state->velocity * sSinceLast;
    worldBoundaries.clamp(state->location.z, state->location.x);
}

int MovingObject::serialize(uint8 *buffer, int start, int buflength) const
{
    using ap::net::serialize;
    int length = 0;
    length += ap::net::serialize(id, buffer, start+length, buflength-length);
    length += ap::net::serialize(objectType, buffer, start+length, buflength+length);
    length += state->serialize( buffer, start+length, buflength-length);
    //length += control->serialize( buffer, start+length, buflength-length);
    length += worldBoundaries.serialize(buffer, start+length, buflength-length);
    return length;
}

int MovingObject::unserialize(uint8 *buffer, int start)
{
    using ap::net::unserialize;
    int length = 0;
    length += ap::net::unserialize(id, buffer, start+length);
    length += ap::net::unserialize(objectType, buffer, start+length);
    length += state->unserialize(buffer, start+length);
    //length += control->unserialize(buffer, start+length);
    length += worldBoundaries.unserialize(buffer, start+length);
    return length;
}

net::NetObject *MovingObject::create(uint32 id) {}

bool MovingObject::testCollision(const MovingObject &other) const
{ return this->state->testCollision(*(other.getStatePtr())); }


} // namespace ap

