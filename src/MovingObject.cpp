#include "MovingObject.hpp"

#include <assert.h>
#include <Ogre.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <Carbon/Carbon.h>
#endif

#include "net/serializer.hpp"

namespace ap {

MovingObject::MovingObject(float nFriction, Ogre::Vector3 startingVelocity):
    initialFacing(Ogre::Vector3::UNIT_X),
    state(MovableState(startingVelocity)),
    control(MovableControl()),
    friction(nFriction),
    worldBoundaries(0.0f, 0.0f, 0.0f, 0.0f),
    maxSpeedSquared(625.0),
    pOwnerNode(0)
{}

MovingObject::~MovingObject() {}

void MovingObject::setWorldBoundaries(float top, float bottom, float left, float right)
{
    worldBoundaries = RectBoundaries(top, bottom, left, right);
}

void MovingObject::setMaxSpeed(float speed)
{
    assert(speed > 0);
    maxSpeedSquared = speed * speed;
}

/**
 * Asettaa nopeuden, tarkoitus käyttää alkunopeuden asettamiseksi.
 *
 * TODO: Tämän tulisi olla vain palvelimen tehtävissä.
 */
void MovingObject::setVelocity(Ogre::Vector3 newVelocity)
{
    state.velocity = newVelocity;
}

void MovingObject::addForwardAcceleration(float amount)
{
    float maxAbsAcceleration = 25;
    control.accelerationFwd += amount;

    if(control.accelerationFwd > maxAbsAcceleration) {
        control.accelerationFwd = maxAbsAcceleration;
    } else if (control.accelerationFwd < (-maxAbsAcceleration)) {
        control.accelerationFwd = 0-maxAbsAcceleration;
    }
}

void MovingObject::addClockwiseTurningSpeed(float amount)
{
    control.velocityCWiseTurning += amount;
}


/**
 * Returns the direction this object is facing.
 *
 * See below for detailed explanation:
 * http://www.ogre3d.org/wiki/index.php/Quaternion_and_Rotation_Primer#Resetting_Orientation
 *
 * return   Ogre::Vector3
 */
Ogre::Vector3 MovingObject::getFacing() const { return state.orientation * initialFacing; }

void MovingObject::update(unsigned long msSinceLast)
{
    if (!pOwnerNode)
        return;

    updateFacing(msSinceLast);
    updateVelocity(msSinceLast);
    updatePosition(msSinceLast);
}

void MovingObject::updateVelocity(unsigned long msSinceLast)
{
    assert (0 != pOwnerNode);

    // Friction
    state.velocity = state.velocity - (friction * msSinceLast * 0.001f);

    state.velocity += control.accelerationFwd * msSinceLast * 0.001f * getFacing();
    if ( state.velocity.squaredLength() > maxSpeedSquared ) {
        state.velocity = state.velocity / state.velocity.squaredLength() * maxSpeedSquared;
    }

}

void MovingObject::updateFacing(unsigned long msSinceLast)
{
    assert( 0 != pOwnerNode );

    if(control.velocityCWiseTurning != 0) {
        Ogre::Radian turning = Ogre::Radian(control.velocityCWiseTurning * msSinceLast * 0.001f);
        pOwnerNode->rotate(Ogre::Vector3::UNIT_Y, turning, Ogre::Node::TS_LOCAL);
        state.orientation = pOwnerNode->getOrientation();
    }
}

void MovingObject::updatePosition(unsigned long msSinceLast)
{
    assert( 0 != pOwnerNode );

    state.location = state.location + state.velocity * msSinceLast * 0.001f;
    worldBoundaries.clamp(state.location.z, state.location.x);
    pOwnerNode->setPosition(state.location);
}

int MovingObject::serialize(uint8 *buffer, int start, int buflength) const
{
    int length = 0;
    length += state.serialize( buffer, start+length, buflength-length);
    length += control.serialize( buffer, start+length, buflength-length);
    length += worldBoundaries.serialize(buffer, start+length, buflength-length);
    return length;
}

int MovingObject::unserialize(uint8 *buffer, int start)
{
    int length = 0;
    length += state.unserialize(buffer, start+length);
    length += control.unserialize(buffer, start+length);
    length += worldBoundaries.unserialize(buffer, start+length);
    return length;
}

net::NetObject *MovingObject::create(int id) {}

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

} // namespace ap

