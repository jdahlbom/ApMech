#ifndef AP_MAINMENUSTATE_H
#define AP_MAINMENUSTATE_H

#ifndef WIN32
#include <Ogre.h>
#else
#include <Ogre/Ogre.h>
#endif

#include "GameState.h"

#include "Gui.h"
#include "GuiMainMenuReceiver.h"


namespace ap {
  class MenuStateViewport;

  class MainMenuState : public GameState, public GuiMainMenuReceiver{
public:
    MainMenuState( GameStateManager *gameStateManager,
		   Ogre::SceneManager *sceneManager,
		   ap::Gui *gui);
    ~MainMenuState( void );

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

    // GuiMainMenuReceiver
    void receiveQuitGame();
    void receiveConfigureKeyboard();
    void receivePlayGame();

private:
    ap::Gui                     *pGui;
    ap::MenuStateViewport       *mViewport;
    Ogre::SceneManager          *pSceneManager;

    void createGUIWindow();
    void terminateGUIWindow();
};

} // namespace ap
#endif
