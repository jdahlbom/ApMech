#include "KeyConfMenuState.h"

#include "ActionKeyMap.h"
#include "GameStateManager.h"
#include "Gui.h"
#include "MenuStateViewport.h"
#include "net/netdata.h"

namespace ap {
  KeyConfMenuState::KeyConfMenuState(ap::GameStateManager *gameStateManager,
				     Ogre::SceneManager *sceneManager,
				     ap::Gui *gui, ap::ActionKeyMap *akMap):
    pGui(gui),
    pActionKMap(akMap),
    pSceneMgr(sceneManager),
    mViewport(NULL)
  {
    assert(NULL != pGui);
    initStateManager(gameStateManager);
  }

  void KeyConfMenuState::enter( void ) {
    assert(NULL != pGui);
    pGui->setupKeyConfWindow();
    mViewport = new MenuStateViewport(pSceneMgr);
    pGui->refreshKeyConfiguration(pActionKMap);
  }

  void KeyConfMenuState::exit( void ) {
    assert(NULL != pGui);
    pGui->hideKeyConfWindow();
    delete(mViewport);
  }

  void KeyConfMenuState::pause( void ) {}
  void KeyConfMenuState::resume( void ) {}
  void KeyConfMenuState::update( unsigned long lTimeElapsed ) {}

  bool KeyConfMenuState::keyPressed( const ooinput::KeyEvent &e )
  {
    if(pGui->keyPressed(e))
      return true;

    IngameAction action = pActionKMap->getActionForKey(e.key);

    switch( action ) {
    case QUIT_GAME:
      this->getStateManager()->leaveKeyConfMenu();
      break;
    default:
      // Intentionally empty.
      break;
    }
    return true;
  }
  bool KeyConfMenuState::keyReleased( const ooinput::KeyEvent &e ) {
    if(pGui->keyReleased(e))
      return true;

    return true;
  }

  bool KeyConfMenuState::mousePressed(const ooinput::MouseClickedEvent &e) {
    if (pGui->mousePressed(e))
      return true;
    return true;
  }
  bool KeyConfMenuState::mouseReleased(const ooinput::MouseClickedEvent &e) {
    if (pGui->mouseReleased(e))
      return true;
    return true;
  }
  bool KeyConfMenuState::mouseMoved(const ooinput::MouseMovedEvent &e) {
    if (pGui->mouseMoved(e))
      return true;
    return true;
  }

} // namespace ap
