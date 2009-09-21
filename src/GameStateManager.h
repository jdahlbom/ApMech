/**
 * Most of the code was lifted from a wiki-linked example application:
 * http://www.ogre3d.org/wiki/index.php/Using_OIS
 * http://dev.neoharmonix.net/ois-example.zip
 **/

#ifndef AP_GAMESTATEMANAGER_H
#define AP_GAMESTATEMANAGER_H

#include <OgreRoot.h>
#include <OgreConfigFile.h>

#include "ooinput/InputSystem.h"
#include "ooinput/KeyboardListener.h"
#include "ooinput/MouseListener.h"

namespace ap {

class Gui;
class GameState;
class PlayState;

//-----------------------------------------------------------------------------
 class GameStateManager : public ap::ooinput::KeyboardListener,
  public ap::ooinput::MouseListener {
public:
    GameStateManager(
		Ogre::Root *root,
                ooinput::InputSystem *inputSystem,
                Ogre::SceneManager *sceneManager,
		ap::Gui *gui);
    ~GameStateManager( void );

    void startGame();

    //    enum GAMESTATE {INTRO, PLAY};
    enum GAMESTATE {PLAY, LOGIN};
    GameState *getStatePtr(GAMESTATE state);

    void changeState( GameState *gameState );
    void pushState( GameState *gameState );
    void popState( void );
    void requestShutdown( void );

    bool keyPressed( const ooinput::KeyEvent &e );
    bool keyReleased( const ooinput::KeyEvent &e );
    bool mousePressed( const ap::ooinput::MouseClickedEvent &e);
    bool mouseReleased( const ap::ooinput::MouseClickedEvent &e);
    bool mouseMoved( const ap::ooinput::MouseMovedEvent &e);


    Ogre::Root		    *mRoot;
    Ogre::SceneManager      *mSceneMgr;
    ap::Gui                 *pGui;
    ooinput::InputSystem    *pInputMgr;

    PlayState               *mPlayState;

    bool bShutdown;
    std::vector<GameState*> mStates;
    std::map<GAMESTATE, GameState*> *mStateMap;
};

} // namespace ap

#endif
