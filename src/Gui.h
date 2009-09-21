#ifndef AP_GUI
#define AP_GUI

#include <CEGUI.h>

#include "ooinput/KeyboardListener.h"
#include "ooinput/KeyEvent.h"

#include "ooinput/MouseClickedEvent.h"
#include "ooinput/MouseListener.h"
#include "ooinput/MouseMovedEvent.h"


namespace ap {

class Gui
{
 public:
  Gui(CEGUI::Renderer *renderer);
  ~Gui();

  void setupChatBox();


  // KeyboardListener interface
  /**
   * Inform this object that key was pressed.
   *
   * @return   bool  True if keypress was handled in GUI, false otherwise.
   */
  bool keyPressed(const ap::ooinput::KeyEvent &event);

  /**
   * Inform this object that key was released.
   *
   * @return   bool  True if keyrelease was handled in GUI, false otherwise.
   */
  bool keyReleased(const ap::ooinput::KeyEvent &event);

  // MouseListener interface

  bool mousePressed(const ap::ooinput::MouseClickedEvent &event);
  bool mouseReleased(const ap::ooinput::MouseClickedEvent &event);
  bool mouseMoved(const ap::ooinput::MouseMovedEvent &event);

 private:
  CEGUI::System *mSystem;
  CEGUI::Window *pRoot;
  int  keysBeingPressed;

  CEGUI::uint MapKeyToCEGUI(ap::ooinput::KeySymbol key);
};

} // namespace ap
#endif
