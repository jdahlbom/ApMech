#include "RectBoundaries.hpp"

#include "Serializer.hpp"

namespace Ap {

RectBoundaries::RectBoundaries(float _top, float _bottom, float _left, float _right) :
    top(_top),
    bottom(_bottom),
    left(_left),
    right(_right) {}

RectBoundaries::RectBoundaries(const RectBoundaries &source) :
    top(source.top),
    bottom(source.bottom),
    left(source.left),
    right(source.right) {}

void RectBoundaries::clamp(float &vert, float &horiz) const
{
    if( horiz < left ) {
        horiz = left;
    } else if (horiz > right) {
        horiz = right;
    }

    if ( vert < bottom) {
        vert = bottom;
    } else if ( vert > top) {
        horiz = top;
    }
}

// NetObject

/**
 * @return  int     Number of byts written to buffer.
 */
int RectBoundaries::serialize(enet_uint8 buffer[], int start, int buflength) const {
    int length = 0;
    length += serializer::serialize(top, buffer, start+length, buflength-length);
    length += serializer::serialize(bottom, buffer, start+length, buflength-length);
    length += serializer::serialize(left, buffer, start+length, buflength-length);
    length += serializer::serialize(right, buffer, start+length, buflength-length);
    return length;
}

/**
 * @return  int     Number of byts read from buffer.
 */
int RectBoundaries::unserialize(enet_uint8 buffer[], int start) {
    int length = 0;
    length += serializer::unserialize(top, buffer, start+length);
    length += serializer::unserialize(bottom, buffer, start+length);
    length += serializer::unserialize(left, buffer, start+length);
    length += serializer::unserialize(right, buffer, start+length);
    return length;
}
} // namespace Ap
