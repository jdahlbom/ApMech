namespace Ap {

RectBoundaries::RectBoundaries(float _top, float _bottom, float _left, float _right) {
    top = _top;
    bottom = _bottom;
    left = _left;
    right = _right;
}

RectBoundaries::RectBoundaries(const RectBoundaries &source) {
    top = source.top;
    bottom = source.bottom;
    left = source.left;
    right = source.right;
}

// NetObject
// FIXME! Floating point numbers will really cause interoperability issues between platforms!
// http://www.parashift.com/c++-faq-lite/serialization.html#faq-36.6

/**
 * @return  int     Number of byts written to buffer.
 */
int serialize(enet_uint8 buffer[], int start, int buflength) {
    int length = 0;
    *(float *)(buffer+start+length) = top;      length += 4;
    *(float *)(buffer+start+length) = bottom;   length += 4;
    *(float *)(buffer+start+length) = left;     length += 4;
    *(float *)(buffer+start+length) = right;    length += 4;
    return length;
}

/**
 * @return  int     Number of byts read from buffer.
 */
int unserialize(enet_uint8 buffer[], int start) {
    int length = 0;
    top     = buffer + start + length;  length += 4;
    bottom  = buffer + start + length;  length += 4;
    left    = buffer + start + length;  length += 4;
    right   = buffer + start + length;  length += 4;
    return length;
}
}
