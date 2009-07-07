#ifndef AP_RECTBOUNDARIES
#define AP_RECTBOUNDARIES

#include "net/Serializable.h"

namespace ap {

class RectBoundaries : public net::Serializable {
  public:
    float   top;
    float   bottom;
    float   left;
    float   right;

    RectBoundaries(float top, float bottom, float left, float right);
    ~RectBoundaries() {}
    RectBoundaries(const RectBoundaries &source);

    void clamp(float &vert, float &horiz) const;

    // NetObject
    int serialize(uint8 buffer[], int start, int buflength) const;
    int unserialize(uint8 buffer[], int start) ;
};
} // namespace ap
#endif
