#include "rotations.hpp"

namespace ap {
namespace math {

/**
 * Returns the rotation matrix resulting from yaw, starting from initial position of UNIT_Z
 *
 * Ogre uses right handed coordinate system, so commonly found formulas need to be
 * converted form left handed.
 */
Ogre::Matrix3 getRotationMatrixAfterYaw(Ogre::Radian yaw)
{
    Ogre::Real rotations [3][3];

    Ogre::Real sinValue = Ogre::Math::Sin(yaw);
    Ogre::Real cosValue = Ogre::Math::Cos(yaw);

    rotations[0][0] = cosValue;
    rotations[0][1] = 0;
    rotations[0][2] = sinValue;
    rotations[1][0] = 0;
    rotations[1][1] = 1;
    rotations[1][2] = 0;
    rotations[2][0] = 0 - sinValue;
    rotations[2][1] = 0;
    rotations[2][2] = cosValue;

    return Ogre::Matrix3(rotations);
}

}
}
