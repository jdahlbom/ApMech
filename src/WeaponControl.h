#ifndef AP_WEAPON_CONTROL_H
#define AP_WEAPON_CONTROL_H

#include "types.h"
#include "net/Controller.h"
#include "net/serializer.h"
#include <iostream>

namespace ap {

class WeaponControl : public net::Controller {
    public:
    static const uint8 NOT_FIRING = 0;
    static const uint8 IS_FIRING = 1;

    WeaponControl(uint32 msFiringInterval = 1000);
    ~WeaponControl() {}

    void setFiring(bool firingStatus);

    int serialize(uint8 buffer[], int start, int buflength) const;
    int unserialize(uint8 buffer[], int start);

    void dump();

    // Should be in state:
    bool fire(uint64 tstamp);

    private:
    uint8   isFiring;
    uint64  lastFired;
    uint32  firingInterval;
}; // class WeaponControl
} // namespace ap


#endif
