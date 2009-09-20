#ifndef AP_MOUSELISTENER
#define AP_MOUSELISTENER

#include "MouseClickedEvent.h"
#include "MouseMovedEvent.h"

namespace ap {
namespace ooinput {

class MouseListener
{
    public:
    virtual ~MouseListener() { }
    virtual bool mousePressed(const MouseClickedEvent &event)=0;
    virtual bool mouseReleased(const MouseClickedEvent &event)=0;
    virtual bool mouseMoved(const MouseMovedEvent &event)=0;
};

} //namespace ooinput
} //namespace ap

#endif
