#include <assert.h>
#include <Ogre.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <Carbon/Carbon.h>
#endif

#include "MovingObject.hpp"


namespace Ap {

MovingObject::MovingObject(float nFriction, Ogre::Vector3 startingVelocity):
    location(Ogre::Vector3::ZERO),
    orientation(Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z),
    initialFacing(Ogre::Vector3::UNIT_X),
    velocity(startingVelocity),
    accelerationFwd(0),
    velocityCWiseTurning(0),
    friction(nFriction),
    limitTop(0),
    limitBottom(0),
    limitLeft(0),
    limitRight(0),
    maxSpeedSquared(625.0),
    pOwnerNode(0)
{}

MovingObject::~MovingObject() {}

void MovingObject::setWorldBoundaries(float top, float bottom, float left, float right)
{
    limitTop = top;
    limitBottom = bottom;
    limitLeft = left;
    limitRight = right;
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
    velocity = newVelocity;
}

void MovingObject::addForwardAcceleration(float amount)
{
    float maxAbsAcceleration = 25;
    accelerationFwd += amount;

    if(accelerationFwd > maxAbsAcceleration) {
        accelerationFwd = maxAbsAcceleration;
    } else if (accelerationFwd < (-maxAbsAcceleration)) {
        accelerationFwd = 0-maxAbsAcceleration;
    }
}

void MovingObject::addClockwiseTurningSpeed(float amount)
{
    velocityCWiseTurning += amount;
}


/**
 * Returns the direction this object is facing.
 *
 * See below for detailed explanation:
 * http://www.ogre3d.org/wiki/index.php/Quaternion_and_Rotation_Primer#Resetting_Orientation
 *
 * return   Ogre::Vector3
 */
Ogre::Vector3 MovingObject::getFacing() const { return orientation * initialFacing; }

void MovingObject::update(unsigned long msSinceLast)
{
    if (!pOwnerNode)
        return;

    updateFacing(msSinceLast);
    updateVelocity(msSinceLast);
    updatePosition(msSinceLast);

/*
    std::cout << "[VELOCITY] x=" << velocity.x << ", z=" << velocity.z << std::endl;
    std::cout << "[POSITION] x=" << location.x << ", y=" << location.y << ", z="<<location.z << std::endl;
    std::cout << "[ACCELERATION] " << accelerationFwd << std::endl; */
}

void MovingObject::updateVelocity(unsigned long msSinceLast)
{
    assert (0 != pOwnerNode);

    // Friction
    velocity = velocity * (1.0f -friction * msSinceLast * 0.001f);

    velocity += accelerationFwd * msSinceLast * 0.001f * getFacing();
    if ( velocity.squaredLength() > maxSpeedSquared ) {
        velocity = velocity / velocity.squaredLength() * maxSpeedSquared;
    }

}

void MovingObject::updateFacing(unsigned long msSinceLast)
{
    assert( 0 != pOwnerNode );

    if(velocityCWiseTurning != 0) {
        Ogre::Radian turning = Ogre::Radian(velocityCWiseTurning * msSinceLast * 0.001f);
        pOwnerNode->rotate(Ogre::Vector3::UNIT_Y, turning, Ogre::Node::TS_LOCAL);
        orientation = pOwnerNode->getOrientation();
    }
}

void MovingObject::updatePosition(unsigned long msSinceLast)
{
    assert( 0 != pOwnerNode );

    location = location + velocity * msSinceLast * 0.001f;
    if( location.x < limitLeft ) {
        location.x = limitLeft;
    } else if (location.x > limitRight) {
        location.x = limitRight;
    }

    if (location.z < limitBottom) {
        location.z = limitBottom;
    } else if (location.z > limitTop) {
        location.z = limitTop;
    }
    pOwnerNode->setPosition(location);
}

}
