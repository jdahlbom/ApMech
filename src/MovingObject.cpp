#include "MovingObject.hpp"

#include <assert.h>
#include <Ogre.h>

#include "net/serializer.hpp"
#include "math/rotations.hpp"
#include "types.h"
#include "constants.h"

namespace ap {

MovingObject::MovingObject(float nFriction, Ogre::Vector3 startingVelocity):
    objectType(ap::OBJECT_TYPE_UNDEFINED),
    initialFacing(Ogre::Vector3::UNIT_X),
    state(new MovableState(startingVelocity)),
    control(new MovableControl()),
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

void MovingObject::setMaxSpeed(float speed)
{
    if(speed < 0)
        return;
    maxSpeedSquared = speed * speed;
}

/**
 * Asettaa nopeuden, tarkoitus käyttää alkunopeuden asettamiseksi.
 *
 * TODO: Tämän tulisi olla vain palvelimen tehtävissä.
 */
void MovingObject::setVelocity(Ogre::Vector3 newVelocity)
{
    state->velocity = newVelocity;
}

void MovingObject::addForwardAcceleration(float amount)
{
    float maxAbsAcceleration = 25;
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


/**
 * Returns the direction this object is facing.
 *
 * See below for detailed explanation:
 * http://www.ogre3d.org/wiki/index.php/Quaternion_and_Rotation_Primer#Resetting_Orientation
 *
 * return   Ogre::Vector3
 */
Ogre::Vector3 MovingObject::getFacing() const { return state->orientation * initialFacing; }

void MovingObject::update(unsigned long msSinceLast)
{
    updateFacing(msSinceLast);
    updateVelocity(msSinceLast);
    updatePosition(msSinceLast);
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

void MovingObject::updateVelocity(unsigned long msSinceLast)
{
    // Friction
    Ogre::Real length = state->velocity.length();
    if( 0.1f < length && friction != 0.0f) {
        state->velocity = Ogre::Vector3::ZERO;
    } else if(length > 0.0f) {
        Ogre::Vector3 unitVelocity = state->velocity / length;
        state->velocity = state->velocity - unitVelocity*(friction * msSinceLast * 0.001f);

        // Friction must not be able to create a backwards motion!
        if(unitVelocity.dotProduct(state->velocity) < 0) {
            state->velocity = Ogre::Vector3::ZERO;
        }
    }

    state->velocity += control->accelerationFwd * msSinceLast * 0.001f * getFacing();
    Ogre::Real squaredLength = state->velocity.squaredLength();
    if ( squaredLength > maxSpeedSquared && squaredLength > 0.0f) {
        state->velocity = (state->velocity / squaredLength) * maxSpeedSquared;
    }
}

void MovingObject::updateFacing(unsigned long msSinceLast)
{
    if(control->velocityCWiseTurning != 0) {
        Ogre::Radian turning = Ogre::Radian(control->velocityCWiseTurning * msSinceLast * 0.001f);
        state->ccwRotation += turning;
        Ogre::Matrix3 rotMatrix(ap::math::getRotationMatrixAfterYaw(state->ccwRotation));
        state->orientation = Ogre::Quaternion(rotMatrix);
    }
}

void MovingObject::updatePosition(unsigned long msSinceLast)
{
    state->location = state->location + state->velocity * msSinceLast * 0.001f;
    worldBoundaries.clamp(state->location.z, state->location.x);
}

int MovingObject::serialize(uint8 *buffer, int start, int buflength) const
{
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

