#include "CombinedControls.h"

#include "MovableControl.h"
#include "WeaponControl.h"

#include "types.h"

namespace ap {
CombinedControls::CombinedControls(MovableControl *move,
        WeaponControl *firing,
        TurretControl *turret):
    moveControl(move),
    firingControl(firing),
    turretControl(turret)
{
    assert(0 != moveControl);
    assert(0 != firingControl);
    assert(0 != turretControl);
}

int CombinedControls::serialize(uint8 buffer[], int start, int buflength) const {
    assert(0 != moveControl);
    assert(0 != firingControl);
    assert(0 != turretControl);

    uint32 length = 0;
    length += moveControl->serialize(buffer, start+length, buflength-length);
    length += firingControl->serialize(buffer, start+length, buflength-length);
    length += turretControl->serialize(buffer, start+length, buflength-length);
    return length;
}
int CombinedControls::unserialize(uint8 buffer[], int start) {
    assert(0 != moveControl);
    assert(0 != firingControl);
    assert(0 != turretControl);

    uint32 length = 0;
    length += moveControl->unserialize(buffer, start+length);
    length += firingControl->unserialize(buffer, start+length);
    length += turretControl->unserialize(buffer, start+length);

    return length;
}

void CombinedControls::reset()
{
    moveControl->reset();
    firingControl->reset();
    turretControl->reset();
}

void CombinedControls::dump() {
    moveControl->dump();
    firingControl->dump();
    turretControl->dump();
}

} // namespace ap
