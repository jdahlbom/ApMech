#ifndef AP_RECTBOUNDARIES
#define AP_RECTBOUNDARIES

#include "net/NetObject.h"

namespace ap {

class RectBoundaries : public net::NetObject {
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
    int serialize(enet_uint8 buffer[], int start, int buflength) const;
    int unserialize(enet_uint8 buffer[], int start) ;
    NetObject *create(int id) { /*TODO: should there be anything here anyways? */}
};
} // namespace ap
#endif
