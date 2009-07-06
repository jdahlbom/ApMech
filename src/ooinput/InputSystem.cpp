#include "InputSystem.h"
#include "KeyboardListener.h"

#include <list>

namespace ap {
namespace ooinput {

InputSystem::InputSystem()
{
    //Alustetaan listener-säiliöt.
    mKeyListeners = new std::list<KeyboardListener*>;
}

InputSystem::~InputSystem()
{
    //Poistetaan listener-säiliöt.
    std::list<KeyboardListener*>::iterator it;
    for(it=mKeyListeners->begin(); it!=mKeyListeners->end();++it) {
        *it=0;
    }

    delete(mKeyListeners);
}


void InputSystem::addKeyboardListener(KeyboardListener *keyListener)
{
    //Lisätään annettu listeneri säiliöön.
    mKeyListeners->push_back(keyListener);
}

void InputSystem::removeKeyboardListener(KeyboardListener *keyListener)
{
    //Poistetaan annettu listeneri säiliöstä.
    mKeyListeners->remove(keyListener);
}

void InputSystem::fireKeyPressedEvent(const KeyEvent &event)
{
    //Kutsutaan keyPressed:iä kaikille säiliön listenereille.
    std::list<KeyboardListener*>::iterator it;
    for(it=mKeyListeners->begin(); it!=mKeyListeners->end(); ++it) {
        static_cast<KeyboardListener*>(*it)->keyPressed(event);
    }
}

void InputSystem::fireKeyReleasedEvent(const KeyEvent &event)
{
    //Kutsutaan keyReleased:ia kaikille säiliön listenereille.
    std::list<KeyboardListener*>::iterator it;
    for(it=mKeyListeners->begin(); it!=mKeyListeners->end(); ++it) {
        static_cast<KeyboardListener*>(*it)->keyReleased(event);
    }
}

} //namespace ooinput
} //namespace ap
