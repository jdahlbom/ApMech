#ifndef AP_MATH_ROTATIONS
#define AP_MATH_ROTATIONS

#include <OgreMatrix3.h>
#include <OgreMath.h>

namespace ap {
namespace math {

Ogre::Matrix3 getRotationMatrixAfterYaw(Ogre::Radian yaw);

}
}

#endif
