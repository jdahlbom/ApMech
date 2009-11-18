#ifndef AP_GUI
#define AP_GUI

#include <CEGUI/CEGUI.h>
#include <list>

#include "ActionKeyMap.h"
#include "GuiChatReceiver.h"
#include "GuiLoginReceiver.h"
#include "GuiMainMenuReceiver.h"
#include "ScoreListing.h"
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

  void setupLimboWindow();
  void setVisibleLimboMenu(bool visible);

  void setupKeyConfWindow(ActionKeyMap *pActionKMap);
  void hideKeyConfWindow();

  void setupScoreWindow();
  void showScoreWindow();
  void hideScoreWindow();
  void updateScores(const ScoreListing &scores);

  void setupMainMenuWindow();
  void hideMainMenuWindow();
  void setMainMenuReceiver(GuiMainMenuReceiver *gMMReceiver);

  void setupChatBox();
  void activateChatBox(bool activate = true);

  void setChatReceiver(GuiChatReceiver *receiver);
  void addChatItem(const std::string &item);
  void addConsoleItem(const std::string &item);

  void setupLoginWindow(const std::string &ipAddress = "", 
			const std::string &playerName = "", 
			const float colorIndex = 0.0f);
  void hideLoginWindow();
  void setLoginReceiver(GuiLoginReceiver *receiver);
  float getColorSliderValue();

  // KeyboardListener interface

  bool keyPressed(const ap::ooinput::KeyEvent &event);
  bool keyReleased(const ap::ooinput::KeyEvent &event);

  // MouseListener interface

  bool mousePressed(const ap::ooinput::MouseClickedEvent &event);
  bool mouseReleased(const ap::ooinput::MouseClickedEvent &event);
  bool mouseMoved(const ap::ooinput::MouseMovedEvent &event);

 private:
  CEGUI::System       *mSystem;
  CEGUI::Window       *pRoot;
  CEGUI::WindowManager *pWinMgr;

  CEGUI::Window       *pChatBox;
  int                 keysBeingPressed;
  GuiChatReceiver     *pChatReceiver;
  GuiLoginReceiver    *pLoginReceiver;
  GuiMainMenuReceiver *pMainMenuReceiver;
  ActionKeyMap        *pActionKMap;

  std::string         keyConfWaitingForKey;

  std::list<ap::uint32> scoreListUIDs;

  CEGUI::uint MapKeyToCEGUI(ap::ooinput::KeySymbol key);
  bool chatMessageSent(const CEGUI::EventArgs &args);
  bool attemptLogin(const CEGUI::EventArgs &args);
  bool requestQuit(const CEGUI::EventArgs &args);

  bool keyConfSelectionChanged(const CEGUI::EventArgs &args);
  void refreshKeyConfiguration(const ActionKeyMap *const pActionKMap);

  bool mainMenuPlay(const CEGUI::EventArgs &args);  
  bool mainMenuQuit(const CEGUI::EventArgs &args);
  bool mainMenuConfKeyboard(const CEGUI::EventArgs &args);

  void setupFileLoadedWindow(const std::string &windowName,
			     const std::string &layoutFile);
  void hideNamedWindow(const std::string &windowName);
  CEGUI::Window *getNamedWindowPtr(const std::string &windowName);

  static const std::string loginLayoutFile;
  static const std::string loginRootName;
  static const std::string loginNameField;
  static const std::string loginAddressField;
  static const std::string loginConsoleField;

  static const std::string chatLayoutFile;
  static const std::string chatRootName;

  static const std::string scoreWindowName;
  static const std::string scoreLayoutFile;

  static const std::string limboWindowName;
  static const std::string limboListName;
  static const std::string limboLayoutFile;

  static const std::string keyConfLayoutFile;
  static const std::string keyConfWindowName;

  static const std::string mainMenuWindowName;
  static const std::string mainMenuLayoutFile;
};

} // namespace ap
#endif
