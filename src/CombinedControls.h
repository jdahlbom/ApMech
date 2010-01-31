#ifndef AP_COMBINED_CONTROLS_H
#define AP_COMBINED_CONTROLS_H

#include "types.h"
#include "net/Controller.h"
#include "MovableControl.h"
#include "WeaponControl.h"
#include "TurretControl.h"

namespace ap {
class MovableControl;

class CombinedControls : public net::Controller
{
    public:
    CombinedControls(MovableControl *move,
            WeaponControl *firing, TurretControl *turret);
    ~CombinedControls(){}
    int serialize(uint8 buffer[], int start, int buflength) const;
    int unserialize(uint8 buffer[], int start);

    void dump();
    void reset();

    private:
    MovableControl  *moveControl;
    WeaponControl   *firingControl;
    TurretControl   *turretControl;
}; // class CombinedControls
} // namespace ap

#endif
