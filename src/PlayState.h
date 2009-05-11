#ifndef AP_PLAYSTATE_H
#define AP_PLAYSTATE_H

#include <OgreCamera.h>
#include <CEGUI.h>

#include "GameState.h"

namespace Ap {

class PlayState : public GameState {
public:
    PlayState( GameStateManager *gameStateManager,
                Ogre::SceneManager *pSceneManager);
    ~PlayState( void );

    void enter( void );
    void exit( void );

    void pause( void );
    void resume( void );
    void update( unsigned long lTimeElapsed );

    bool keyPressed( const Ap::KeyEvent &e );
    bool keyReleased( const Ap::KeyEvent &e );

    void createGUIWindow();

private:
    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::Viewport       *mViewport;

    Ogre::SceneManager   *pSceneManager;

    CEGUI::Window       *mStateOverlay;
};

} // namespace Ap
#endif
