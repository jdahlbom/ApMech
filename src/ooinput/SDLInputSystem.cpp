#include <SDL.h>

#include <iostream>

#include "SDLInputSystem.h"
#include "KeyEvent.h"

namespace Ap {

SDLInputSystem::SDLInputSystem()
{
    SDL_EnableUNICODE(1);
}
SDLInputSystem::~SDLInputSystem() {}

void SDLInputSystem::capture()
{
    // Poll for events
    while(SDL_PollEvent(&sdlEvent))
    {
        // Process
        switch(sdlEvent.type) {
            // If type=keyboard, fire keybevents
            case SDL_KEYDOWN:
                fireKeyPressedEvent( createEvent(sdlEvent.key.keysym, AP_E_KEYDOWN) );
                break;
            case SDL_KEYUP:
                fireKeyReleasedEvent( createEvent(sdlEvent.key.keysym, AP_E_KEYUP) );
                break;
            default:
                break;
        }
    }
}

KeyEvent SDLInputSystem::createEvent(SDL_keysym &keysym, KeyEventType type)
{
    KeyEvent event;
    event.type = type;
    event.unicode = keysym.unicode;
    event.key = mapKey(keysym.sym);

    //TODO: Poista tämä kun testailut on testailtu.
    std::cout << "Event: type: " << type << ", unicode: " << event.unicode << ", key: " << event.key << std::endl;

    return event;
}

KeySymbol SDLInputSystem::mapKey(SDLKey key)
{
    switch(key) {
        case SDLK_ESCAPE:
            return AP_K_ESCAPE;
            break;
        case SDLK_a:
            return AP_K_a;
            break;
        case SDLK_SPACE:
            return AP_K_SPACE;
            break;
        default:
            return AP_K_UNDEF;
            break;
    }
}

} // namespace Ap
