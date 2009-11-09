/**
 * Most of the code was lifted from a wiki-linked example application:
 * http://www.ogre3d.org/wiki/index.php/Using_OIS
 * http://dev.neoharmonix.net/ois-example.zip
 **/

#ifndef AP_LIMBOSTATE_H
#define AP_LIMBOSTATE_H

#include "ooinput/KeyboardListener.h"
#include "ooinput/MouseListener.h"
#include "GameState.h"

#include "Gui.h"
#include "net/netdata.h"

namespace ap {

class ActionKeyMap;
class GameStateManager;

class LimboState: public GameState {
public:
  LimboState(GameStateManager *gameStateManager,
	     ap::Gui *gui,
	     ap::net::NetData *netdata,
	     ap::ActionKeyMap *akMap);
  ~LimboState( void ) { }

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
  ap::Gui           *pGui;
  ap::net::NetData  *pNetData;
  ap::ActionKeyMap  *pActionKMap;

  void selectVehicleType(const std::string &vehicleType);
};

} // namespace ap

#endif
