#ifndef AP_OOINPUT_MOUSEMOVEDEVENT
#define AP_OOINPUT_MOUSEMOVEDEVENT

#include "../types.h"


namespace ap {
namespace ooinput {

  // Taken from SDL_MouseMotionEvent
  // Do we even have "state" from other input systems?
  // I suggest not using button state in application until we know this..
typedef struct{  
  ap::uint8    state; // button state .. requires more work - is a bitmask in SDL.
  ap::uint16   xabs;  // absolute x coordinate (within container window?)
  ap::uint16   yabs;  // see above, but for "y"
  ap::uint16   xrel;  // relative x coordinate (change to previous position? or container?)
  ap::uint16   yrel;  // see above, but for "y"
} MouseMovedEvent;

} //namespace ooinput
} //namespace ap

#endif
