#include "WeaponControl.h"

namespace ap {
WeaponControl::WeaponControl(uint32 msFiringInterval):
    isFiring(NOT_FIRING),
    lastFired(0),
    firingInterval(msFiringInterval) {}

void WeaponControl::setFiring(bool firingStatus) {
    if(firingStatus) {
        isFiring = IS_FIRING;
    } else {
        isFiring = NOT_FIRING;
    }
}

int WeaponControl::serialize(uint8 buffer[], int start, int buflength) const {
    return ap::net::serialize(isFiring, buffer, start, buflength);
}

int WeaponControl::unserialize(uint8 buffer[], int start) {
    return ap::net::unserialize(isFiring, buffer, start);
}

bool WeaponControl::fire(uint64 tstamp) {
    if( IS_FIRING==isFiring && ((tstamp-lastFired) > firingInterval) ) {
        lastFired = tstamp;
        return true;
    }
    return false;
}

void WeaponControl::dump() {
    std::cout << "[WEAPONCONTROL::dump] isFiring: " << ((int)isFiring) << std::endl;
}

} // namespace ap
