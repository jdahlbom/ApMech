#include "serializer.hpp"

#include <Ogre.h>

namespace ap {
namespace net {
int serialize(const Ogre::Vector3 &vect, uint8 *buffer, int start, int buflength)
{
    // TODO: Vector3 is composed of 3 Real:s. Real is by default a float, but with DOUBLE_PRECISION-flag
    // Ogre will use doubles instead. Should be taken into account!
    int length = 0;
    length += serialize(static_cast<float>(vect.x), buffer, start+length, buflength-length);
    length += serialize(static_cast<float>(vect.y), buffer, start+length, buflength-length);
    length += serialize(static_cast<float>(vect.z), buffer, start+length, buflength-length);
    return length;
}

int serialize(const Ogre::Quaternion &quat, uint8 *buffer, int start, int buflength)
{
    Ogre::Radian rad = Ogre::Radian();
    Ogre::Vector3 vect = Ogre::Vector3::UNIT_X;
    quat.ToAngleAxis(rad, vect);

    int length = 0;
    length += serialize(static_cast<float>(rad.valueRadians()), buffer, start+length, buflength-length);
    length += serialize(vect, buffer, start+length, buflength-length);
    return length;
}

// FIXME! Floating point numbers will really cause interoperability issues between platforms!
// http://www.parashift.com/c++-faq-lite/serialization.html#faq-36.6
int serialize(float f, uint8 *buffer, int start, int buflength)
{
    *(reinterpret_cast<float *>(buffer+start)) = f;
    return 4; // Size of float is 4 bytes
}

int unserialize(Ogre::Vector3 &vect, uint8 *buffer, int start)
{
    // TODO: Vector3 is composed of 3 Real:s. Real is by default a float, but with DOUBLE_PRECISION-flag
    // Ogre will use doubles instead. Should be taken into account!
    int length = 0;
    float x,y,z;
    length += unserialize(x, buffer, start+length);
    length += unserialize(y, buffer, start+length);
    length += unserialize(z, buffer, start+length);
    vect.x = x;
    vect.y = y;
    vect.z = z;
    return length;
}

int unserialize(Ogre::Quaternion &quat, uint8 *buffer, int start)
{
    int length = 0;
    float radians = 0;

    Ogre::Vector3 vect = Ogre::Vector3::UNIT_X;
    length += unserialize(radians, buffer, start+length);
    length += unserialize(vect, buffer, start + length);
    quat.FromAngleAxis(Ogre::Radian(radians), vect);

    return length;
}

// FIXME! Floating point numbers will really cause interoperability issues between platforms!
// http://www.parashift.com/c++-faq-lite/serialization.html#faq-36.6
int unserialize(float &f, uint8 *buffer, int start)
{
    f = *(reinterpret_cast<float *>(buffer+start));
    return 4; // 4 is the length of float in bytes
}

} // namespace net
} // namespace ap
