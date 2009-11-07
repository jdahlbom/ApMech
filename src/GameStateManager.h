/**
 * Most of the code was lifted from a wiki-linked example application:
 * http://www.ogre3d.org/wiki/index.php/Using_OIS
 * http://dev.neoharmonix.net/ois-example.zip
 **/

#ifndef AP_GAMESTATEMANAGER_H
#define AP_GAMESTATEMANAGER_H

#ifndef WIN32
#include <Ogre.h>
#else
#include <Ogre/Ogre.h>
#endif

#include "net/netdata.h"
#include "ooinput/InputSystem.h"
#include "ooinput/KeyboardListener.h"
#include "ooinput/MouseListener.h"

namespace ap {

class Gui;
class GameState;
class PlayState;
class LoginState;

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
    void requestShutdown( void );
    void loginToGame(net::NetData *netdata, const std::string &playerName);

    // input listeners
    bool keyPressed( const ooinput::KeyEvent &e );
    bool keyReleased( const ooinput::KeyEvent &e );
    bool mousePressed( const ap::ooinput::MouseClickedEvent &e);
    bool mouseReleased( const ap::ooinput::MouseClickedEvent &e);
    bool mouseMoved( const ap::ooinput::MouseMovedEvent &e);

 private:
    Ogre::Root		    *mRoot;
    Ogre::SceneManager      *mSceneMgr;
    ap::Gui                 *pGui;
    ooinput::InputSystem    *pInputMgr;

    LoginState              *mLoginState;
    PlayState               *mPlayState;

    bool                    bShutdown;
    std::vector<GameState*> mStates;

    // state management
    void changeState( GameState *gameState );
    void pushState( GameState *gameState );
    void popState( void );
};

} // namespace ap

#endif
