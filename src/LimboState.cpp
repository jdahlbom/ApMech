#include "LimboState.h"
#include "GameStateManager.h"
#include "Gui.h"
#include "net/netdata.h"

namespace ap {
  LimboState::LimboState(ap::GameStateManager *gameStateManager,
	     ap::Gui *gui, ap::net::NetData *netdata):
    pGui(gui),
    pNetData(netdata)
  {
    assert(NULL != pGui);
    initStateManager(gameStateManager);
    pGui->setupLimboWindow();
  }

  void LimboState::enter( void ) {
    assert(NULL != pGui);
    pGui->setVisibleLimboMenu(true);
  }

  void LimboState::exit( void ) {
    assert(NULL != pGui);
    pGui->setVisibleLimboMenu(false);
  }

  void LimboState::pause( void ) {}
  void LimboState::resume( void ) {}
  void LimboState::update( unsigned long lTimeElapsed ) {}

  bool LimboState::keyPressed( const ooinput::KeyEvent &e )
  {
    if(pGui->keyPressed(e))
      return true;

    switch(e.key) {
    case(ooinput::AP_K_l):
      getStateManager()->leaveLimboMenu();
      break;
    default:
      // Intentionally empty.
      break;
    }
    return true;
  }
  bool LimboState::keyReleased( const ooinput::KeyEvent &e ) {
    if(pGui->keyReleased(e))
      return true;

    return true;
  }

  bool LimboState::mousePressed(const ooinput::MouseClickedEvent &e) {
    if (pGui->mousePressed(e))
      return true;
    return true;
  }
  bool LimboState::mouseReleased(const ooinput::MouseClickedEvent &e) {
    if (pGui->mouseReleased(e))
      return true;
    return true;
  }
  bool LimboState::mouseMoved(const ooinput::MouseMovedEvent &e) {
    if (pGui->mouseMoved(e))
      return true;
    return true;
  }
} // namespace ap
