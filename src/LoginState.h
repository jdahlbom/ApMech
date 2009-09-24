#ifndef AP_LOGINSTATE_H
#define AP_LOGINSTATE_H

#include <fstream>
#include <string>
#include <Ogre.h>

#include "GameState.h"

#include "Gui.h"
#include "GuiLoginReceiver.h"

#include "types.h"
#include "constants.h"


namespace ap {

  class LoginState : public GameState, public GuiLoginReceiver{
public:
    LoginState( GameStateManager *gameStateManager,
		Ogre::SceneManager *sceneManager,
	       ap::Gui *gui);
    ~LoginState( void );

    void enter( void );
    void exit( void );

    void pause( void ) {};
    void resume( void ) {};
    void update( unsigned long lTimeElapsed ) {};



//--KeyboardListener-(inherited through GameState)-----------------------------
    bool keyPressed( const ooinput::KeyEvent &e );
    bool keyReleased( const ooinput::KeyEvent &e );

    // MouseListener
    bool mousePressed(const ooinput::MouseClickedEvent &e);
    bool mouseReleased(const ooinput::MouseClickedEvent &e);
    bool mouseMoved(const ooinput::MouseMovedEvent &e);

    // GuiLoginReceiver
    void attemptLogin(const std::string &ipAddress, const std::string &playerName);
    void requestQuit();

private:
    ap::Gui                     *pGui;
    Ogre::Camera                *mCamera;
    Ogre::SceneManager          *pSceneManager;

    void createGUIWindow();
    void terminateGUIWindow();
    void setupOgreViewport(Ogre::SceneManager *sceneMgr);
    void removeCamera(Ogre::SceneManager *sceneMgr);
};

} // namespace ap
#endif
