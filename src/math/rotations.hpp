#ifndef AP_MATH_ROTATIONS
#define AP_MATH_ROTATIONS

#ifndef WIN32
#include <OgreMath.h>
#include <OgreMatrix3.h>
#else
#include <Ogre/OgreMath.h>
#include <Ogre/OgreMatrix3.h>
#endif

namespace ap {
namespace math {

Ogre::Matrix3 getRotationMatrixAfterYaw(Ogre::Radian yaw);

}
}

#endif
