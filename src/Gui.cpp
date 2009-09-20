#include "Gui.h"

#include <CEGUI.h>

namespace ap {

  Gui::Gui(CEGUI::Renderer *renderer)
  {
    assert(renderer);
    using namespace CEGUI;
    
    mSystem = new System( renderer );
    SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
    
    mSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
    mSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
    
    WindowManager *mWin = WindowManager::getSingletonPtr();
    pRoot = mWin->createWindow("DefaultGUISheet", "root");    
    mSystem->setGUISheet(pRoot);
  } // Gui::Gui    

  Gui::~Gui() 
  {
    delete(mSystem);
  }

  void Gui::setupChatBox() 
  {
    CEGUI::WindowManager *winMgr = CEGUI::WindowManager::getSingletonPtr();
    CEGUI::Window *chatLayout = winMgr->loadWindowLayout("ChatBox.layout");
    pRoot->addChildWindow(chatLayout);
  }
}
