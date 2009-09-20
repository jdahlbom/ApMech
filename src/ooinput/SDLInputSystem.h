#ifndef AP_OOINPUT_SDLINPUTSYSTEM
#define AP_OOINPUT_SDLINPUTSYSTEM

#include <SDL.h>

#include "InputSystem.h"
#include "KeyEvent.h"
#include "MouseClickedEvent.h"
#include "MouseMovedEvent.h"

namespace ap {
namespace ooinput {

class SDLInputSystem: public InputSystem
{
    public:
    SDLInputSystem();
    ~SDLInputSystem();
    void capture();

    private:
    static KeyEvent createKeyEvent(SDL_keysym &keysym, KeyEventType type);
    static KeySymbol mapKey(SDLKey key);

    static MouseClickedEvent createMouseClickedEvent(SDL_MouseButtonEvent &event);
    static MouseMovedEvent createMouseMovedEvent(SDL_MouseMotionEvent &event);
    SDL_Event sdlEvent;
};

} //namespace ooinput
} //namespace ap

#endif
