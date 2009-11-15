#ifndef AP_KEYCONFMENUSTATE_H
#define AP_KEYCONFMENUSTATE_H

#include "ooinput/KeyboardListener.h"
#include "ooinput/MouseListener.h"
#include "GameState.h"

#include "Gui.h"

namespace Ogre {
  class SceneManager;
}

namespace ap {

class ActionKeyMap;
class GameStateManager;
class MenuStateViewport;

class KeyConfMenuState: public GameState {
public:
  KeyConfMenuState(GameStateManager *gameStateManager,
		   Ogre::SceneManager *sceneManager,
		   ap::Gui *gui,
		   ap::ActionKeyMap *akMap);
  ~KeyConfMenuState( void ) { }

  void enter( void );
  void exit( void );
  
  void pause( void );
  void resume( void );
  void update( unsigned long lTimeElapsed );
  
  bool keyPressed( const ooinput::KeyEvent &e );
  bool keyReleased( const ooinput::KeyEvent &e );
  
  bool mousePressed(const ooinput::MouseClickedEvent &e);
  bool mouseReleased(const ooinput::MouseClickedEvent &e);
  bool mouseMoved(const ooinput::MouseMovedEvent &e);

private:
  ap::Gui               *pGui;
  ap::ActionKeyMap      *pActionKMap;
  Ogre::SceneManager    *pSceneMgr;
  ap::MenuStateViewport *mViewport;
};

} // namespace ap

#endif
