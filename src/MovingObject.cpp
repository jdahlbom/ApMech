#include "MovingObject.h"

#include <assert.h>
#ifndef WIN32
#include <Ogre.h>
#else
#include <Ogre/Ogre.h>
#endif

#include "net/serializer.h"
#include "math/rotations.h"
#include "types.h"
#include "constants.h"

#include "MovableControl.h"
#include "WeaponControl.h"
#include "CombinedControls.h"
#include "ObjectDataModel.h"

namespace ap {

MovingObject::MovingObject(float nFriction, Ogre::Vector3 startingVelocity,
        ObjectDataModel *model, uint8 type_id):
    clamped(false),
    objectType(ap::OBJECT_TYPE_UNDEFINED),
    friction(nFriction),
    maxSpeedSquared(0.0),
    initialFacing(-Ogre::Vector3::UNIT_Z),
    state(new MovableState(startingVelocity)),
    control(new MovableControl()),
    weaponControl(new WeaponControl()),
    combinedControls(new CombinedControls(control, weaponControl)),
    pOwnerNode(0),
    pEntity(0),
    model(model)
{
    id = 0;
    uid = 0;

    if (model) {
        maxForwardAcceleration = model->getMaxForwardAcceleration(type_id);
        maxBackwardAcceleration = model->getMaxBackwardAcceleration(type_id);
        turningRate = model->getMaxTurningRate(type_id);
    }
}

MovingObject::~MovingObject()
{
    delete(state);
    delete(combinedControls);
    delete(weaponControl);
    delete(control);
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
    control->accelerationFwd += amount;
}

void MovingObject::setForwardAcceleration(float amount)
{
    if (amount < -1.0f) {
        amount = -1.0f;
    } else if (amount > 1.0f) {
        amount = 1.0f;
    }

    if (amount < 0.0f) {
        control->accelerationFwd = amount * maxBackwardAcceleration;
    } else {
        control->accelerationFwd = amount * maxForwardAcceleration;
    }
}

void MovingObject::setClockwiseTurningSpeed(float amount)
{
    if (amount < -1.0f) {
        amount = -1.0f;
    } else if (amount > 1.0f) {
        amount = 1.0f;
    }

    control->velocityCWiseTurning = amount * turningRate;
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
   state->orientation = axes[2].getRotationTo(-f);
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
        if ((maxSpeedSquared != 0.0) && (squaredLength > maxSpeedSquared) && (squaredLength > 0.0f))
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
}

int MovingObject::serialize(uint8 *buffer, int start, int buflength) const
{
    using ap::net::serialize;
    int length = 0;
    length += ap::net::serialize(id, buffer, start+length, buflength-length);
    length += ap::net::serialize(objectType, buffer, start+length, buflength+length);
    length += state->serialize( buffer, start+length, buflength-length);
    //length += control->serialize( buffer, start+length, buflength-length);
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
    return length;
}

bool MovingObject::testCollision(const MovingObject &other) const
{ return this->state->testCollision(*(other.getStatePtr())); }


} // namespace ap

