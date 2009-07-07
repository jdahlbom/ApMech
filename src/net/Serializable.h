#ifndef AP_NET_SERIALIZABLE_H
#define AP_NET_SERIALIZABLE_H

typedef unsigned char uint8;

namespace ap {
namespace net {

  /**
   * Interface for all AP-objects that are supposed to be serialized.
   **/
class Serializable {
 public:
    virtual int serialize(uint8 buffer[], int start, int buflength) const = 0;
    virtual int unserialize(uint8 buffer[], int start) = 0;
};

} // namespace net
} // namespace ap
#endif
