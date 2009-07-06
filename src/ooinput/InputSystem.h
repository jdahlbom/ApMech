#ifndef AP_OOINPUT_INPUTSYSTEM
#define AP_OOINPUT_INPUTSYSTEM

#include <list>

#include "KeyboardListener.h"
#include "KeyEvent.h"

namespace ap {
namespace ooinput {

class InputSystem
{
    public:
    InputSystem();
    virtual ~InputSystem();

    virtual void capture( void )=0;
    void addKeyboardListener(KeyboardListener *keyListener);
    void removeKeyboardListener(KeyboardListener *keyListener);

    protected:
    void fireKeyPressedEvent(const KeyEvent &event);
    void fireKeyReleasedEvent(const KeyEvent &event);
    std::list<KeyboardListener*> *mKeyListeners;

    private:

};

} // namespace ooinput
} // namespace ap

#endif
