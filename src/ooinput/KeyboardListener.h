#ifndef AP_KEYBOARDLISTENER
#define AP_KEYBOARDLISTENER

#include "KeyEvent.h"

namespace Ap {

class KeyboardListener
{
    public:
    virtual ~KeyboardListener() { }
    virtual bool keyPressed(const Ap::KeyEvent &event)=0;
    virtual bool keyReleased(const Ap::KeyEvent &event)=0;
};

} // namespace Ap
#endif
