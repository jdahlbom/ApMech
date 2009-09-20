#include "InputSystem.h"

#include <assert.h>
#include <list>

#include "KeyboardListener.h"
#include "KeyEvent.h"
#include "MouseListener.h"
#include "MouseClickedEvent.h"
#include "MouseMovedEvent.h"

namespace ap {
namespace ooinput {

InputSystem::InputSystem()
{
    mKeyListeners = new std::list<KeyboardListener*>;
    mMouseListeners = new std::list<MouseListener*>;
}

InputSystem::~InputSystem()
{
  mKeyListeners->clear();
  mMouseListeners->clear();

  delete(mKeyListeners);
  delete(mMouseListeners);
}


void InputSystem::addKeyboardListener(KeyboardListener *keyListener)
{
  assert(keyListener != 0);
  mKeyListeners->push_back(keyListener);
}

void InputSystem::removeKeyboardListener(KeyboardListener *keyListener)
{
  mKeyListeners->remove(keyListener);
}

void InputSystem::addMouseListener(MouseListener *mouseListener)
{
  assert(mouseListener != 0);
  mMouseListeners->push_back(mouseListener);
}
  
  void InputSystem::removeMouseListener(MouseListener *mouseListener)
  {
    mMouseListeners->remove(mouseListener);
  }
  
  void InputSystem::fireKeyPressedEvent(const KeyEvent &event)
  {
    std::list<KeyboardListener*>::iterator it;
    for(it=mKeyListeners->begin(); it!=mKeyListeners->end(); ++it) {
      static_cast<KeyboardListener*>(*it)->keyPressed(event);
    }
  }
  
  void InputSystem::fireKeyReleasedEvent(const KeyEvent &event)
  {
    std::list<KeyboardListener*>::iterator it;
    for(it=mKeyListeners->begin(); it!=mKeyListeners->end(); ++it) {
      static_cast<KeyboardListener*>(*it)->keyReleased(event);
    }
  }

  void InputSystem::fireMousePressedEvent(const MouseClickedEvent &event)
  {
    std::list<MouseListener*>::iterator it;
    for(it=mMouseListeners->begin(); it!=mMouseListeners->end(); ++it) {
      static_cast<MouseListener*>(*it)->mousePressed(event);
    }
  }

  void InputSystem::fireMouseReleasedEvent(const MouseClickedEvent &event)
  {
    std::list<MouseListener*>::iterator it;
    for(it=mMouseListeners->begin(); it!=mMouseListeners->end(); ++it) {
      static_cast<MouseListener*>(*it)->mouseReleased(event);
    }
  }

  void InputSystem::fireMouseMovedEvent(const MouseMovedEvent &event)
  {
    std::list<MouseListener*>::iterator it;
    for(it=mMouseListeners->begin(); it!=mMouseListeners->end(); ++it) {
      static_cast<MouseListener*>(*it)->mouseMoved(event);
    }
  }

} //namespace ooinput
} //namespace ap
