/**
 * Most of the code was lifted from a wiki-linked example application:
 * http://www.ogre3d.org/wiki/index.php/Using_OIS
 * http://dev.neoharmonix.net/ois-example.zip
 **/

#ifndef AP_SDLGAMESTATEMANAGER_H
#define AP_SDLGAMESTATEMANAGER_H

#include <Ogre.h>

#include "GameStateManager.h"

#include "ooinput/InputSystem.h"

namespace ap {

//-----------------------------------------------------------------------------
class SDLGameStateManager : public GameStateManager {
public:
    SDLGameStateManager(
		Ogre::Root *root,
                ooinput::InputSystem *inputSystem,
                Ogre::SceneManager *sceneManager) :
    GameStateManager(root, inputSystem, sceneManager)
      {}
    ~SDLGameStateManager( void );
protected:
    void updateGraphics();
}; // class SDLGameStateManager

} // namespace ap

#endif
