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
    bool    clamped;

    RectBoundaries(float _top, float _bottom, float _left, float _right);
    ~RectBoundaries() {}
    RectBoundaries(const RectBoundaries &source);

    void setBoundaries(float _top, float _bottom, float _left, float _right);
    void clamp(float &vert, float &horiz);

    // NetObject
    int serialize(uint8 buffer[], int start, int buflength) const;
    int unserialize(uint8 buffer[], int start) ;
};
} // namespace ap
#endif
