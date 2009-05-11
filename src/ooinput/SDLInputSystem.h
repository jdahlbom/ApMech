#ifndef AP_SDLINPUTSYSTEM
#define AP_SDLINPUTSYSTEM

#include <SDL.h>

#include "InputSystem.h"
#include "KeyEvent.h"

namespace Ap {

class SDLInputSystem: public InputSystem
{
    public:
    SDLInputSystem();
    ~SDLInputSystem();
    void capture();

    private:
    static KeyEvent createEvent(SDL_keysym &keysym, KeyEventType type);
    static KeySymbol mapKey(SDLKey key);
    SDL_Event sdlEvent;

};

} // namespace Ap

#endif
