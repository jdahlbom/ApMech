#include "Gui.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>

#include "ActionKeyMap.h"
#include "definitions.h"
#include "functions.h"
#include "GuiChatReceiver.h"
#include "GuiLoginReceiver.h"
#include "GuiMainMenuReceiver.h"
#include "GuiMouseReceiver.h"
#include "ScoreListing.h"
#include "ooinput/KeyboardListener.h"
#include "ooinput/KeyEvent.h"
#include "ooinput/MouseClickedEvent.h"
#include "ooinput/MouseListener.h"
#include "ooinput/MouseMovedEvent.h"

namespace ap {

#if AP_PLATFORM == AP_PLATFORM_OSX
  std::string mResourcePath = ap::bundlePath() + "/Contents/Resources/Media/gui/layout/";
#else
  std::string mResourcePath = "";
#endif

  const std::string Gui::loginLayoutFile = mResourcePath + "Login.layout";
  const std::string Gui::loginRootName = "LoginRoot";
  const std::string Gui::loginNameField = "/Login/Name";
  const std::string Gui::loginAddressField = "/Login/Address";
  const std::string Gui::loginConsoleField = "/Login/Console";

  const std::string Gui::chatLayoutFile = mResourcePath + "ChatBox.layout";
  const std::string Gui::chatRootName = "ChatBoxRoot";

  const std::string Gui::scoreLayoutFile = mResourcePath + "ScoreBoard.layout";
  const std::string Gui::scoreWindowName = "ScoreList";

  const std::string Gui::limboLayoutFile = mResourcePath + "Limbo.layout";
  const std::string Gui::limboWindowName = "Limbo";
  const std::string Gui::limboListName = "Limbo/List";

  const std::string Gui::keyConfLayoutFile = mResourcePath + "KeyConfigurationMenu.layout";
  const std::string Gui::keyConfWindowName = "KeyConfiguration";

  const std::string Gui::mainMenuLayoutFile = mResourcePath + "MainMenu.layout";
  const std::string Gui::mainMenuWindowName = "MainMenu";

  Gui::Gui(CEGUI::Renderer *renderer) :
    mSystem(0),
    keysBeingPressed(0),
    pChatReceiver(0),
    pLoginReceiver(0),
    pMainMenuReceiver(0),
    pMouseReceiver(0),
    currentState(TARGET_NOT_AVAILABLE),
    pActionKMap(0),
    keyConfWaitingForKey(""),
    scoreListUIDs(std::list<ap::uint32>())
  {
    assert(renderer);
    using namespace CEGUI;
    
    try {    
        mSystem = &CEGUI::System::create( *renderer );
    } catch (CEGUI::GenericException genException) {
  	std::cout << "Could not create CEGUI::System!" << std::endl;
	std::cout << genException.getMessage() << std::endl;
	throw; // GUI cannot function without CEGUI, rethrow the exception.
    }
    assert(mSystem);
	
    // JL 4.6.2011, porting to CEGUI 0.7.5 
    //SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"ApMechLookSkin.scheme");
    SchemeManager::getSingleton().create((CEGUI::utf8*)"ApMechLookSkin.scheme");

    mSystem->setDefaultMouseCursor((CEGUI::utf8*)"ApMechLook", (CEGUI::utf8*)"MouseArrow");
    mSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");

    pWinMgr = WindowManager::getSingletonPtr();
    pRoot = pWinMgr->createWindow("DefaultGUISheet", "root");
    mSystem->setGUISheet(pRoot);
  } // Gui::Gui

  Gui::~Gui()
  {
    // JL 4.6.2011, to compile with CEGUI 0.7.5 
    //delete(mSystem);
    CEGUI::OgreRenderer::destroySystem();

  }

  void Gui::setupChatBox()
  {
    assert(pWinMgr);
    CEGUI::Window *chatLayout = pWinMgr->loadWindowLayout(chatLayoutFile);
    pRoot->addChildWindow(chatLayout);

    pChatBox = pWinMgr->getWindow("/ChatBox/Text");
    pChatBox->subscribeEvent(CEGUI::Editbox::EventTextAccepted,
			     CEGUI::Event::Subscriber(&Gui::chatMessageSent, this));
  }

  void Gui::setupLoginWindow(const std::string &ipAddress, const std::string &playerName, const float colorIndex)
  {
    assert(pWinMgr);
    if ( pWinMgr->isWindowPresent(loginRootName) ) {
      CEGUI::Window *loginRoot = pWinMgr->getWindow(loginRootName);
      loginRoot->show();
      loginRoot->activate();
      return;
    }

    CEGUI::Window *loginLayout = pWinMgr->loadWindowLayout(loginLayoutFile);
    pRoot->addChildWindow(loginLayout);

    CEGUI::Window *loginButton = getNamedWindowPtr("/Login/LoginButton");
    assert(loginButton != NULL);
    loginButton->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&Gui::attemptLogin, this));

    CEGUI::Window *quitButton = getNamedWindowPtr("/Login/QuitButton");
    assert(quitButton != NULL);

    quitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			       CEGUI::Event::Subscriber(&Gui::requestQuit, this));

    pWinMgr->getWindow("/Login/Address")->setText(ipAddress);
    pWinMgr->getWindow("/Login/Name")->setText(playerName);
    dynamic_cast<CEGUI::Slider *>(pWinMgr->getWindow("/Login/ColorSlider"))->setCurrentValue(colorIndex);
  }

  void Gui::setupScoreWindow() {
    setupFileLoadedWindow(scoreWindowName,
			  scoreLayoutFile);
  }

  /**
   * Helper method to create a named window.
   * @param Name of the window
   * @param Name of the layout file defining the window.
   **/
  void Gui::setupFileLoadedWindow(const std::string &windowName,
				  const std::string &layoutFile)
  {
    assert(pRoot);
    assert(pWinMgr);
    CEGUI::Window *windowRoot = NULL;
    if ( pWinMgr->isWindowPresent(windowName) ) {
      windowRoot = pWinMgr->getWindow(windowName);
    } else {
      windowRoot = pWinMgr->loadWindowLayout(layoutFile);
      pRoot->addChildWindow(windowRoot);
    }
    windowRoot->hide();
    windowRoot->deactivate();
  }

  void Gui::hideNamedWindow(const std::string &windowName) {
    assert(pWinMgr);
    if ( pWinMgr->isWindowPresent(windowName) ) {
     CEGUI::Window *windowRoot = pWinMgr->getWindow(windowName);
     windowRoot->hide();
    }
  }

  /**
   * Returns a pointer to a named window, or NULL if no such window exists.
   */
  CEGUI::Window *Gui::getNamedWindowPtr(const std::string &windowName) {
    assert(pWinMgr != NULL);
    if (pWinMgr->isWindowPresent(windowName)) {
      return pWinMgr->getWindow(windowName);
    }
    return NULL;
  }

  // KEY CONFIGURATION MENU:
  void Gui::setupKeyConfWindow(ActionKeyMap *pAKMap) {
    pActionKMap = pAKMap;
    setupFileLoadedWindow(keyConfWindowName,
			  keyConfLayoutFile);
    CEGUI::Window *confWindow = getNamedWindowPtr(keyConfWindowName);

    // input handling here..

    confWindow->show();
    confWindow->activate();

    using CEGUI::MultiColumnList;
    using CEGUI::Event;
    MultiColumnList *multiCL = dynamic_cast<MultiColumnList *>(confWindow);
    assert(NULL != multiCL);
    multiCL->subscribeEvent(MultiColumnList::EventSelectionChanged,
			    Event::Subscriber(&Gui::keyConfSelectionChanged, this));
    multiCL->setUserSortControlEnabled(false);
    refreshKeyConfiguration(pActionKMap);
  }

  void Gui::hideKeyConfWindow() {
    hideNamedWindow(keyConfWindowName);
  }

  void Gui::refreshKeyConfiguration(const ActionKeyMap *const pAKMap)
  {
    using CEGUI::MultiColumnList;
    using CEGUI::ListboxItem;
    using CEGUI::ListboxTextItem;

    MultiColumnList *multiCL = dynamic_cast<MultiColumnList *>(pWinMgr->getWindow(keyConfWindowName));
    assert(NULL != multiCL);
    multiCL->resetList();

    const CEGUI::uint nameCol = 1;
    const CEGUI::uint keyCol = 2;

    std::string actionName;
    CEGUI::uint currentRow;
    CEGUI::colour selColour = CEGUI::colour(0.5f, 0.5f, 0.9f, 1.0f);
    while (pAKMap->getEachActionName(actionName)) {
      currentRow = multiCL->addRow();

      ListboxItem *name = new ListboxTextItem(actionName,0,0,false,true);
      ListboxItem *key = new ListboxTextItem(pAKMap->getMappedKeyName(actionName),0,0,false,true);

      name->setSelectionColours(selColour, selColour, selColour, selColour);
      key->setSelectionColours(selColour, selColour, selColour, selColour);

      try {
        multiCL->setItem(name, nameCol, currentRow);
        multiCL->setItem(key, keyCol, currentRow);
      } catch (CEGUI::InvalidRequestException) {
        std::cout << "Mismatch between Gui::updateScores column IDs and layout file!" << std::endl;
        throw;
      }
    }
  }

  bool Gui::keyConfSelectionChanged(const CEGUI::EventArgs &args) {
    using CEGUI::ListboxItem;
    using CEGUI::MultiColumnList;
    using CEGUI::WindowEventArgs;
    using CEGUI::EventArgs;

    // FIXME: Uh-huh, static casting, no way of checking validity...
    const WindowEventArgs &arg = static_cast<const WindowEventArgs &>(args);
    MultiColumnList *multiCL = dynamic_cast<MultiColumnList *>(arg.window);
    assert(multiCL != NULL);

    ListboxItem *nameItem = multiCL->getFirstSelectedItem();
    if (NULL == nameItem)
      return true;

    ListboxItem *keyItem = multiCL->getNextSelected(nameItem);
    assert(NULL != keyItem); // Should always exist if nameItem exists!

    keyItem->setText("?");
    keyConfWaitingForKey = to_string(nameItem->getText());
    return true;
  }

  // MAIN MENU:
  void Gui::setupMainMenuWindow() {
    setupFileLoadedWindow(mainMenuWindowName,
			  mainMenuLayoutFile);

    CEGUI::Window *playButton = getNamedWindowPtr("MainMenu/Selection/Play");
    assert(playButton != NULL);

    playButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			       CEGUI::Event::Subscriber(&Gui::mainMenuPlay, this));

    CEGUI::Window *quitButton = getNamedWindowPtr("MainMenu/Selection/Quit");
    assert(playButton != NULL);

    quitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			       CEGUI::Event::Subscriber(&Gui::mainMenuQuit, this));

    CEGUI::Window *confButton = getNamedWindowPtr("MainMenu/Selection/Controls");
    assert(confButton != NULL);

    confButton->subscribeEvent(CEGUI::PushButton::EventClicked,
			       CEGUI::Event::Subscriber(&Gui::mainMenuConfKeyboard, this));

    CEGUI::Window *mainMenuWindow = getNamedWindowPtr(mainMenuWindowName);
    mainMenuWindow->show();
    mainMenuWindow->activate();
  }

  void Gui::hideMainMenuWindow() {
    hideNamedWindow(mainMenuWindowName);
  }

  void Gui::setMainMenuReceiver(GuiMainMenuReceiver *gMMReceiver) {
    pMainMenuReceiver = gMMReceiver;
  }

  bool Gui::mainMenuPlay(const CEGUI::EventArgs &args) {
    assert(pMainMenuReceiver);
    pMainMenuReceiver->receivePlayGame();
    return true;
  }

  bool Gui::mainMenuQuit(const CEGUI::EventArgs &args) {
    assert(pMainMenuReceiver);
    pMainMenuReceiver->receiveQuitGame();
    return true;
  }

  bool Gui::mainMenuConfKeyboard(const CEGUI::EventArgs &args) {
    assert(pMainMenuReceiver);
    pMainMenuReceiver->receiveConfigureKeyboard();
    return true;
  }

  // LIMBO MENU:
  void Gui::setupLimboWindow() {
    setupFileLoadedWindow(limboWindowName,
			  limboLayoutFile);
  }

  void Gui::setVisibleLimboMenu(bool visible)
  {
    assert(pRoot);
    assert(pWinMgr);
    CEGUI::Window *limboRoot = NULL;
    if ( pWinMgr->isWindowPresent(limboWindowName) ) {
      limboRoot = pWinMgr->getWindow(limboWindowName);
    } else return;

    if(visible) {
      limboRoot->show();
      limboRoot->activate();
      limboRoot->setModalState(true);
    } else {
      limboRoot->hide();
      limboRoot->setModalState(false);
    }
  }

  void Gui::addLimboVehicle(const std::string &item)
  {
      CEGUI::Listbox *lbox = dynamic_cast<CEGUI::Listbox *>(CEGUI::WindowManager::getSingletonPtr()->getWindow(limboListName));
      assert (lbox);
      CEGUI::ListboxTextItem *ltItem = new CEGUI::ListboxTextItem(CEGUI::String(item));

      // This is supposed to highlight the selected name in the list.
      // Doesn't appear do it, though... wonder why.

      CEGUI::colour green(0.0, 1.0, 0.0, 0.5);
      ltItem->setSelectionColours(green);

      lbox->addItem(ltItem);
      lbox->ensureItemIsVisible(ltItem);
  }

  void Gui::selectLimboVehicle(int index)
  {
    std::cout << "selected limbo vehicle " << index << std::endl;
      CEGUI::Listbox *lbox = dynamic_cast<CEGUI::Listbox *>(CEGUI::WindowManager::getSingletonPtr()->getWindow(limboListName));
      lbox->setItemSelectState(index, true);
  }

  std::string Gui::getLimboVehicle(int index)
  {
      CEGUI::Listbox *lbox = dynamic_cast<CEGUI::Listbox *>(CEGUI::WindowManager::getSingletonPtr()->getWindow(limboListName));
      CEGUI::ListboxItem *ltItem = lbox->getListboxItemFromIndex(index);
      std::string name(ltItem->getText().c_str());

      return name;
  }

void Gui::clearLimboVehicleList()
{
    CEGUI::Listbox *lbox = dynamic_cast<CEGUI::Listbox *>(CEGUI::WindowManager::getSingletonPtr()->getWindow(limboListName));
    if (lbox == NULL) {
      std::cout << "[ERROR] Could not access Limbo menu vehicle listbox by window name!" << std::endl;
      return;
    }

    lbox->resetList();
}

  void Gui::hideLoginWindow() {
    hideNamedWindow(loginRootName);
  }

  void Gui::showScoreWindow()
  {
    assert(pWinMgr);
    if ( pWinMgr->isWindowPresent(scoreWindowName) ) {
      CEGUI::Window *scoreRoot = pWinMgr->getWindow(scoreWindowName);
      scoreRoot->show();
      scoreRoot->deactivate();
      return;
    }
  }

  void Gui::hideScoreWindow() {
    hideNamedWindow(scoreWindowName);
  }

  void Gui::updateScores(const ScoreListing &scores)
  {
    using CEGUI::uint;
    using CEGUI::MultiColumnList;
    using CEGUI::MCLGridRef;
    using CEGUI::ListboxItem;
    using CEGUI::ListboxTextItem;

    assert(pWinMgr);
    MultiColumnList *multiCL = dynamic_cast<MultiColumnList *>(pWinMgr->getWindow(scoreWindowName));

    // matching column indices to the colIDs used in layout file.
    const uint nameCol = 1;
    const uint killCol = 2;
    const uint deathCol = 3;
    const uint scoreCol = 4;


    uint nameIndex, killIndex, deathIndex, scoreIndex;
    try {
      nameIndex = multiCL->getColumnWithID(nameCol);
      killIndex = multiCL->getColumnWithID(killCol);
      deathIndex = multiCL->getColumnWithID(deathCol);
      scoreIndex = multiCL->getColumnWithID(scoreCol);
    } catch (CEGUI::InvalidRequestException) {
      std::cout << "Mismatch between Gui::updateScores column IDs and layout file!" << std::endl;
      throw;
    }

    std::list<ap::uint32> recentUIDList = std::list<ap::uint32>();

    ScoreTuple tuple;
    while(scores.getEachScore(tuple))
      {
	recentUIDList.push_back(tuple.uid);

	uint rowIndex;
	bool addNew = false;
	try {
	  rowIndex = multiCL->getRowWithID(tuple.uid);
	} catch (CEGUI::InvalidRequestException) {
	  // Just means this players' entry is new. New row addition needed.
	  // Cannot set rowIndex to negative since it's uint, use addNew helper variable instead.
	  addNew = true;
	}

	if (addNew) {
	  // Explicitly set the last argument (AutoDelete) to true (even though it's default)
	  ListboxItem *name = new ListboxTextItem(to_string(tuple.nick),0,0,false,true);
	  ListboxItem *kills = new ListboxTextItem(to_string(tuple.kills),0,0,false,true);
	  ListboxItem *deaths = new ListboxTextItem(to_string(tuple.deaths),0,0,false,true);
	  ListboxItem *score = new ListboxTextItem(to_string(tuple.score),0,0,false,true);

	  rowIndex = multiCL->addRow(tuple.uid);

	  try {
	    multiCL->setItem(name, nameCol, rowIndex);
	    multiCL->setItem(kills, killCol, rowIndex);
	    multiCL->setItem(deaths, deathCol, rowIndex);
	    multiCL->setItem(score, scoreCol, rowIndex);
	  } catch (CEGUI::InvalidRequestException) {
	    std::cout << "Mismatch between Gui::updateScores column IDs and layout file!" << std::endl;
	    throw;
	  }

	  scoreListUIDs.push_back(tuple.uid);
	} else {
	  // Update a score row
	  try {
	    // name
	    ListboxItem *item = multiCL->getItemAtGridReference(MCLGridRef(rowIndex,nameIndex));
	    item->setText(to_string(tuple.nick));
	    //kills
	    item = multiCL->getItemAtGridReference(MCLGridRef(rowIndex, killIndex));
	    item->setText(to_string(tuple.kills));
	    //deaths
	    item = multiCL->getItemAtGridReference(MCLGridRef(rowIndex, deathIndex));
	    item->setText(to_string(tuple.deaths));
	    //score
	    item = multiCL->getItemAtGridReference(MCLGridRef(rowIndex, scoreIndex));
	    item->setText(to_string(tuple.score));
	  } catch (CEGUI::InvalidRequestException e) {
	    // ListboxItem at requested MCLGridRef was not found..
	    std::cout << "ListboxItem at requested MCLGridRef was not found, aborting..."
		      << std::endl;
	    throw 0;
	  }
	}
      } // while(scores)

    // Remove score list rows that were NOT in the recent scores
    std::list<ap::uint32>::iterator it;
    for(it=scoreListUIDs.begin(); it!=scoreListUIDs.end(); )
      {
	const ap::uint32 testedId = *it;
	if (std::find(recentUIDList.begin(), recentUIDList.end(), testedId) == recentUIDList.end()){
	  // This row id should no longer be shown in the score list.
	  uint rowIndex;
	  try {
	    rowIndex = multiCL->getRowWithID(testedId);
	  } catch (CEGUI::InvalidRequestException e) {
	    std::cout << "Error removing rows from score list: row not found! "
		      << "Non-fatal, though should not occur!" << std::endl;
	    throw 0;
	  }
	  multiCL->removeRow(rowIndex); // calls ListboxItem destructors on that row.
	  it = scoreListUIDs.erase(it);
	} else {
	  ++it;
	}
      }

        // JL porting 4.6.2011 // multiCL->requestRedraw();
        multiCL->invalidate(); // Update the score list element on next render cycle.
  } // updateScores

  void Gui::activateChatBox(bool activate)
  {
    assert(pChatBox);
    if (activate) pChatBox->activate();
    else pChatBox->deactivate();
  }

  void Gui::setChatReceiver(GuiChatReceiver *receiver)
  {
    assert(receiver);
    pChatReceiver = receiver;
  }

  void Gui::setLoginReceiver(GuiLoginReceiver *receiver)
  {
    assert(receiver);
    pLoginReceiver = receiver;
  }

  void Gui::setMouseReceiver(GuiMouseReceiver *receiver) {
    pMouseReceiver = receiver;
  }

  float Gui::getColorSliderValue()
  {
    CEGUI::Slider *slider = dynamic_cast<CEGUI::Slider *>(CEGUI::WindowManager::getSingletonPtr()->getWindow("/Login/ColorSlider"));
    assert (slider);
    return slider->getCurrentValue();
  }

  void Gui::addChatItem(const std::string &item)
  {
    CEGUI::Listbox *lbox = dynamic_cast<CEGUI::Listbox *>(CEGUI::WindowManager::getSingletonPtr()->getWindow("/ChatBox/List"));
    assert (lbox);
    CEGUI::ListboxTextItem *ltItem = new CEGUI::ListboxTextItem(CEGUI::String(item));
    lbox->addItem(ltItem);
    lbox->ensureItemIsVisible(ltItem);
  }

  void Gui::addConsoleItem(const std::string &item)
  {
    CEGUI::Listbox *lbox = dynamic_cast<CEGUI::Listbox *>(CEGUI::WindowManager::getSingletonPtr()->getWindow(loginConsoleField));
    assert (lbox);
    CEGUI::ListboxTextItem *ltItem = new CEGUI::ListboxTextItem(CEGUI::String(item));
    lbox->addItem(ltItem);
    lbox->ensureItemIsVisible(ltItem);
  }

  // Triggerable events
  bool Gui::chatMessageSent(const CEGUI::EventArgs &args)
  {
    if(pChatReceiver && pChatBox) {
      std::string message(pChatBox->getText().c_str());
      pChatBox->setText("");
      if (message != "") pChatReceiver->sendChatMessage(message);   // send only if not empty
      pChatBox->deactivate();
    }
	return false; // TODO: Why does this function return anything?
  }


  bool Gui::attemptLogin(const CEGUI::EventArgs &args)
  {
    assert(pLoginReceiver);
    assert(pWinMgr);
    CEGUI::Window *address = pWinMgr->getWindow(loginAddressField);
    CEGUI::Window *name = pWinMgr->getWindow(loginNameField);
    if (address->getText() == "" || address->getText() == "0.0.0.0") {
	address->setText("0.0.0.0");
	return true;
      }
    else if (name->getText() == "") {
      name->setText("Nameless newbie");
      return true;
      }
    else {
      pLoginReceiver->attemptLogin(std::string(address->getText().c_str()),
				   std::string(name->getText().c_str()));
    }
    return true;
  }


  bool Gui::requestQuit(const CEGUI::EventArgs &args)
  {
    assert(pLoginReceiver);
    pLoginReceiver->requestQuit();
	return true; // TODO: Why does this function return anything?
  }

  // *****************************************************  Keyboard listener
  /**
   * Inform this object that key was pressed.
   *
   * @return   bool  True if keypress was handled in GUI, false otherwise.
   */
  bool Gui::keyPressed(const ap::ooinput::KeyEvent &event)
  {
    assert(mSystem);

    if ("" != keyConfWaitingForKey) {
      bool isValidKey = pActionKMap->setKeyForAction(event, keyConfWaitingForKey);
      if( !isValidKey )
	return true;

      keyConfWaitingForKey = "";
      refreshKeyConfiguration(pActionKMap);
      return true;
    }

    bool keysymProcessed = false, charProcessed = false;
    keysymProcessed = mSystem->injectKeyDown(MapKeyToCEGUI(event.key));
    if (event.unicode != 0) {
      charProcessed = mSystem->injectChar(event.unicode);
    }

    if (keysymProcessed || charProcessed) {
      ++keysBeingPressed;
      return 1;
    }
    return 0;
  } // keyPressed

  /**
   * Inform this object that key was released.
   *
   * @return   bool  True if keyrelease was handled in GUI, false otherwise.
   */
  bool Gui::keyReleased(const ap::ooinput::KeyEvent &event)
  {
    assert(mSystem);

    bool injectProcessed;
    injectProcessed = mSystem->injectKeyUp(MapKeyToCEGUI(event.key));

    if(injectProcessed)
      return 1;

    // Key releases do not return true from chat box.
    // Count keypresses made in the chat box, and ignore
    // an equal number of keyreleases before returning false.
    if(keysBeingPressed > 0) {
      --keysBeingPressed;
      return 1;
    }
    return 0;
  } // keyReleased


  // MouseListener
  bool Gui::mousePressed(const ap::ooinput::MouseClickedEvent &event)
  {
    using namespace ap::ooinput;

    if("" != keyConfWaitingForKey)
      return true;

    switch(event.button) {
    case AP_B_LEFT: return mSystem->injectMouseButtonDown(CEGUI::LeftButton);
    case AP_B_RIGHT: return mSystem->injectMouseButtonDown(CEGUI::RightButton);
    case AP_B_MIDDLE: return mSystem->injectMouseButtonDown(CEGUI::MiddleButton);
    case AP_B_WHEELDOWN: return mSystem->injectMouseWheelChange(-1);
    case AP_B_WHEELUP: return mSystem->injectMouseWheelChange(1);
    default: break;
    }

    return 0;
  } // mousePressed


  bool Gui::mouseReleased(const ap::ooinput::MouseClickedEvent &event)
  {
    using namespace ap::ooinput;

    if("" != keyConfWaitingForKey)
      return true;

    assert(mSystem);
    // assumption: WHEELUP/WHEELDOWN do not cause released events.
    switch(event.button) {
        case AP_B_LEFT: return mSystem->injectMouseButtonUp(CEGUI::LeftButton);
        case AP_B_RIGHT: return mSystem->injectMouseButtonUp(CEGUI::RightButton);
        case AP_B_MIDDLE: return mSystem->injectMouseButtonUp(CEGUI::MiddleButton);
        default: return 0;
    }
    return 0;
  } // mouseReleased


  void Gui::setTargetState(targetState state)
  {
      CEGUI::utf8 *image = NULL;

    if (state == currentState) {
        // no change in cursor state
        return;
    }

    assert(mSystem);

    switch (state) {
        case TARGET_NOT_AVAILABLE:
            // std::cout << "Cursor changed to TARGET_NOT_AVAILABLE" << std::endl;
            image = (CEGUI::utf8*)"MouseArrow";
            break;
        case TARGET_WITHIN_TORSO_TURN_ANGLE:
            // std::cout << "Cursor changed to TARGET_WITHIN_TORSO_TURN_ANGLE" << std::endl;
            image = (CEGUI::utf8*)"MouseTargetSector";
            break;
        case TARGET_LINED_WITH_TORSO:
            // std::cout << "Cursor changed to TARGET_LINED_WITH_TORSO" << std::endl;
            image = (CEGUI::utf8*)"MouseTargetAcquired";
            break;
        default:
            return;
    }

    CEGUI::MouseCursor::getSingleton().setImage(
            (CEGUI::utf8*)"ApMechLook", image);

    currentState = state;

    return;
  }

  bool Gui::mouseMoved(const ap::ooinput::MouseMovedEvent &event)
  {
    bool retval;
    const CEGUI::Renderer *renderer;

    assert(mSystem);

    renderer = mSystem->getRenderer();

    retval = mSystem->injectMousePosition(static_cast<float>(event.xabs),
					static_cast<float>(event.yabs));

    if (pMouseReceiver) {
        // Tell the interested parties where the mouse is now
/*        pMouseReceiver->receiveMousePosition(
                event.xabs / renderer->getWidth(),
                event.yabs / renderer->getHeight());*/
        CEGUI::Size rendSize = renderer->getDisplaySize(); // JL porting 4.6.2011 //
        pMouseReceiver->receiveMousePosition(
                event.xabs / rendSize.d_width,
                event.yabs / rendSize.d_height);
    }
    return retval;
  }

  CEGUI::uint Gui::MapKeyToCEGUI(ap::ooinput::KeySymbol key)
  {
     using namespace CEGUI;
     using namespace ap::ooinput;

     switch (key)
     {
     case AP_K_BACKSPACE:    return Key::Backspace;
     case AP_K_TAB:          return Key::Tab;
     case AP_K_RETURN:       return Key::Return;
     case AP_K_PAUSE:        return Key::Pause;
     case AP_K_ESCAPE:       return Key::Escape;
     case AP_K_SPACE:        return Key::Space;
     case AP_K_COMMA:        return Key::Comma;
     case AP_K_MINUS:        return Key::Minus;
     case AP_K_PERIOD:       return Key::Period;
     case AP_K_SLASH:        return Key::Slash;
     case AP_K_0:            return Key::Zero;
     case AP_K_1:            return Key::One;
     case AP_K_2:            return Key::Two;
     case AP_K_3:            return Key::Three;
     case AP_K_4:            return Key::Four;
     case AP_K_5:            return Key::Five;
     case AP_K_6:            return Key::Six;
     case AP_K_7:            return Key::Seven;
     case AP_K_8:            return Key::Eight;
     case AP_K_9:            return Key::Nine;
     case AP_K_COLON:        return Key::Colon;
     case AP_K_SEMICOLON:    return Key::Semicolon;
     case AP_K_EQUALS:       return Key::Equals;
     case AP_K_LEFTBRACKET:  return Key::LeftBracket;
     case AP_K_BACKSLASH:    return Key::Backslash;
     case AP_K_RIGHTBRACKET: return Key::RightBracket;
     case AP_K_a:            return Key::A;
     case AP_K_b:            return Key::B;
     case AP_K_c:            return Key::C;
     case AP_K_d:            return Key::D;
     case AP_K_e:            return Key::E;
     case AP_K_f:            return Key::F;
     case AP_K_g:            return Key::G;
     case AP_K_h:            return Key::H;
     case AP_K_i:            return Key::I;
     case AP_K_j:            return Key::J;
     case AP_K_k:            return Key::K;
     case AP_K_l:            return Key::L;
     case AP_K_m:            return Key::M;
     case AP_K_n:            return Key::N;
     case AP_K_o:            return Key::O;
     case AP_K_p:            return Key::P;
     case AP_K_q:            return Key::Q;
     case AP_K_r:            return Key::R;
     case AP_K_s:            return Key::S;
     case AP_K_t:            return Key::T;
     case AP_K_u:            return Key::U;
     case AP_K_v:            return Key::V;
     case AP_K_w:            return Key::W;
     case AP_K_x:            return Key::X;
     case AP_K_y:            return Key::Y;
     case AP_K_z:            return Key::Z;
     case AP_K_DELETE:       return Key::Delete;
     case AP_K_KP0:          return Key::Numpad0;
     case AP_K_KP1:          return Key::Numpad1;
     case AP_K_KP2:          return Key::Numpad2;
     case AP_K_KP3:          return Key::Numpad3;
     case AP_K_KP4:          return Key::Numpad4;
     case AP_K_KP5:          return Key::Numpad5;
     case AP_K_KP6:          return Key::Numpad6;
     case AP_K_KP7:          return Key::Numpad7;
     case AP_K_KP8:          return Key::Numpad8;
     case AP_K_KP9:          return Key::Numpad9;
     case AP_K_KP_PERIOD:    return Key::Decimal;
     case AP_K_KP_DIVIDE:    return Key::Divide;
     case AP_K_KP_MULTIPLY:  return Key::Multiply;
     case AP_K_KP_MINUS:     return Key::Subtract;
     case AP_K_KP_PLUS:      return Key::Add;
     case AP_K_KP_ENTER:     return Key::NumpadEnter;
     case AP_K_KP_EQUALS:    return Key::NumpadEquals;
     case AP_K_UP:           return Key::ArrowUp;
     case AP_K_DOWN:         return Key::ArrowDown;
     case AP_K_RIGHT:        return Key::ArrowRight;
     case AP_K_LEFT:         return Key::ArrowLeft;
     case AP_K_INSERT:       return Key::Insert;
     case AP_K_HOME:         return Key::Home;
     case AP_K_END:          return Key::End;
     case AP_K_PAGEUP:       return Key::PageUp;
     case AP_K_PAGEDOWN:     return Key::PageDown;
     case AP_K_F1:           return Key::F1;
     case AP_K_F2:           return Key::F2;
     case AP_K_F3:           return Key::F3;
     case AP_K_F4:           return Key::F4;
     case AP_K_F5:           return Key::F5;
     case AP_K_F6:           return Key::F6;
     case AP_K_F7:           return Key::F7;
     case AP_K_F8:           return Key::F8;
     case AP_K_F9:           return Key::F9;
     case AP_K_F10:          return Key::F10;
     case AP_K_F11:          return Key::F11;
     case AP_K_F12:          return Key::F12;
     case AP_K_F13:          return Key::F13;
     case AP_K_F14:          return Key::F14;
     case AP_K_F15:          return Key::F15;
     case AP_K_NUMLOCK:      return Key::NumLock;
     case AP_K_SCROLLOCK:    return Key::ScrollLock;
     case AP_K_RSHIFT:       return Key::RightShift;
     case AP_K_LSHIFT:       return Key::LeftShift;
     case AP_K_RCTRL:        return Key::RightControl;
     case AP_K_LCTRL:        return Key::LeftControl;
     case AP_K_RALT:         return Key::RightAlt;
     case AP_K_LALT:         return Key::LeftAlt;
     case AP_K_SYSREQ:       return Key::SysRq;
     case AP_K_MENU:         return Key::AppMenu;
     case AP_K_POWER:        return Key::Power;
     default:                return 0;
     }
     return 0;
  } // MapKeyToCEGUI

} // namespace ap
