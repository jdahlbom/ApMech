#ifndef AP_OOINPUT_MOUSECLICKEDEVENT
#define AP_OOINPUT_MOUSECLICKEDEVENT

#include "../types.h"

namespace ap {
namespace ooinput {

typedef enum {
    AP_E_MOUSEBUTTONDOWN = 1,
    AP_E_MOUSEBUTTONUP = 2,
    AP_E_UNDEFINED = 3
} MouseEventType;

typedef enum {
   AP_B_LEFT = 1,
   AP_B_MIDDLE = 2,
   AP_B_RIGHT = 3,
   AP_B_WHEELDOWN = 4,
   AP_B_WHEELUP = 5,
   AP_B_UNDEFINED = 6
} MouseButton;

typedef struct{
  MouseEventType   type;
  MouseButton      button; // which button was acted upon.
  ap::uint16       x; // Mouse cursor x coordinate at click-time.
  ap::uint16       y; // See above.
} MouseClickedEvent;

} //namespace ooinput
} //namespace ap

#endif
