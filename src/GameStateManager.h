/**
 * Most of the code was lifted from a wiki-linked example application:
 * http://www.ogre3d.org/wiki/index.php/Using_OIS
 * http://dev.neoharmonix.net/ois-example.zip
 **/

#ifndef AP_GAMESTATEMANAGER_H
#define AP_GAMESTATEMANAGER_H

#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>

#include "ooinput/InputSystem.h"

namespace ap {

class GameState;
//class IntroState;
class PlayState;

//-----------------------------------------------------------------------------
class GameStateManager : public ooinput::KeyboardListener {
public:
    GameStateManager( Ogre::Root *root,
                Ogre::RenderWindow *renderWindow,
                ooinput::InputSystem *inputSystem,
                Ogre::SceneManager *sceneManager);
    ~GameStateManager( void );

    void startGame();

    //    enum GAMESTATE {INTRO, PLAY};
    enum GAMESTATE {PLAY, LOGIN};
    GameState *getStatePtr(GAMESTATE state);

    void changeState( GameState *gameState );
    void pushState( GameState *gameState );
    void popState( void );
    void requestShutdown( void );

private:
    bool configureGame( void );
    void setupResources( void );

    bool keyPressed( const ooinput::KeyEvent &e );
    bool keyReleased( const ooinput::KeyEvent &e );

    /*    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    */
    Ogre::Root              *mRoot;
    Ogre::RenderWindow      *mRenderWindow;
    Ogre::SceneManager      *mSceneMgr;
    ooinput::InputSystem    *mInputMgr;

    PlayState           *mPlayState;

    bool bShutdown;
    std::vector<GameState*> mStates;
    std::map<GAMESTATE, GameState*> *mStateMap;
};

} // namespace ap

#endif
