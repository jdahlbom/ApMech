#ifndef AP_KEYBOARDLISTENER
#define AP_KEYBOARDLISTENER

#include "KeyEvent.h"

namespace ap {
namespace ooinput {

class KeyboardListener
{
    public:
    virtual ~KeyboardListener() { }
    virtual bool keyPressed(const KeyEvent &event)=0;
    virtual bool keyReleased(const KeyEvent &event)=0;
};

} //namespace ooinput
} //namespace ap

#endif
