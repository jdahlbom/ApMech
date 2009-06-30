#ifdef AP_RECTBOUNDARIES
#define AP_RECTBOUNDARIES

namespace Ap {

class RectBoundaries : public NetObject {
  public:
    float   top;
    float   bottom;
    float   left;
    float   right;

    RectBoundaries(float top, float bottom, float left, float right);
    ~RectBoundaries() {}
    RectBoundaries(const RectBoundaries &source);

    // NetObject
    int serialize(enet_uint8 buffer[], int start, int buflength);
    int unserialize(enet_uint8 buffer[], int start) ;
    NetObject *create(int id) { /*TODO: should there be anything here anyways? */}
};
} // namespace Ap
#endif
