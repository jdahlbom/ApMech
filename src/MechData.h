#ifndef AP_MECHDATA_H
#define AP_MECHDATA_H

#include "types.h"
#include "constants.h"
#include "net/NetObject.h"

#include <string>

namespace ap {

class MechData : public ap::net::NetObject
{
 public:
    MechData() : name(""), maxTurnRate(0), maxForwardAcceleration(0), maxBackwardAcceleration(0) {};
    ~MechData() {};

    // NetObject interface
    net::NetObject *create(uint32 id) const;
    uint8 getObjectType() const;

    //NetObject serialization
    int serialize(uint8 *buffer, int start, int buflength) const;
    int unserialize(uint8 *buffer, int start);

    int getTurnRate() { return maxTurnRate; }
    int getMaxForwardAcceleration() { return maxForwardAcceleration; }
    int getMaxBackwardAcceleration() { return maxBackwardAcceleration; }
    std::string getName() const { return name; }

    void setTurnRate(int rate) { maxTurnRate = rate; }
    void setMaxForwardAcceleration(int acceleration) { maxForwardAcceleration = acceleration; }
    void setMaxBackwardAcceleration(int acceleration) { maxBackwardAcceleration = acceleration; }
    void setName(std::string name) { this->name = name; }

 private:
    std::string name;
    int maxTurnRate;
    int maxForwardAcceleration;
    int maxBackwardAcceleration;
};

} // namespace ap

#endif
