#ifndef AP_OOINPUT_INPUTSYSTEM
#define AP_OOINPUT_INPUTSYSTEM

#include <list>

#include "KeyboardListener.h"
#include "KeyEvent.h"
#include "MouseListener.h"
#include "MouseMovedEvent.h"
#include "MouseClickedEvent.h"

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

    void addMouseListener(MouseListener *mouseListener);
    void removeMouseListener(MouseListener *mouseListener);

    protected:
    void fireKeyPressedEvent(const KeyEvent &event);
    void fireKeyReleasedEvent(const KeyEvent &event);
    void fireMousePressedEvent(const MouseClickedEvent &event);
    void fireMouseReleasedEvent(const MouseClickedEvent &event);
    void fireMouseMovedEvent(const MouseMovedEvent &event);

    std::list<KeyboardListener*> *mKeyListeners;
    std::list<MouseListener*> *mMouseListeners;
    private:

};

} // namespace ooinput
} // namespace ap

#endif
