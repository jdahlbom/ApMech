#ifndef AP_GUI
#define AP_GUI

#include <CEGUI.h>

#include "GuiChatReceiver.h"
#include "GuiLoginReceiver.h"
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
  void setupLoginWindow(const std::string &ipAddress = "", const std::string &playerName = "");

  void exitLoginWindow();

  void activateChatBox(bool activate = true);

  void setChatReceiver(GuiChatReceiver *receiver);
  void addChatItem(const std::string &item);

  void setLoginReceiver(GuiLoginReceiver *receiver);

  // KeyboardListener interface

  bool keyPressed(const ap::ooinput::KeyEvent &event);
  bool keyReleased(const ap::ooinput::KeyEvent &event);

  // MouseListener interface

  bool mousePressed(const ap::ooinput::MouseClickedEvent &event);
  bool mouseReleased(const ap::ooinput::MouseClickedEvent &event);
  bool mouseMoved(const ap::ooinput::MouseMovedEvent &event);

 private:
  CEGUI::System     *mSystem;
  CEGUI::Window     *pRoot;

  CEGUI::Window     *pChatBox;
  int               keysBeingPressed;
  GuiChatReceiver   *pChatReceiver;
  GuiLoginReceiver  *pLoginReceiver;

  CEGUI::uint MapKeyToCEGUI(ap::ooinput::KeySymbol key);
  bool chatMessageSent(const CEGUI::EventArgs &args);
  bool attemptLogin(const CEGUI::EventArgs &args);
  bool requestQuit(const CEGUI::EventArgs &args);

  static const std::string loginLayoutFile;
  static const std::string loginRootName;
  static const std::string loginNameField;
  static const std::string loginAddressField;

  static const std::string chatLayoutFile;
  static const std::string chatRootName;

};

} // namespace ap
#endif
