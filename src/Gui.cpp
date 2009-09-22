#include "Gui.h"

#include <iostream>

#include <CEGUI.h>

namespace ap {

  Gui::Gui(CEGUI::Renderer *renderer) :
    keysBeingPressed(0),
    pReceiver(0)
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

    pChatBox = winMgr->getWindow("/ChatBox/Text");
    pChatBox->subscribeEvent(CEGUI::Editbox::EventTextAccepted,
			     CEGUI::Event::Subscriber(&Gui::chatMessageSent, this));
  }

  void Gui::activateChatBox(bool activate)
  {
    if (activate) pChatBox->activate();
    else pChatBox->deactivate();
  }

  void Gui::setChatReceiver(GuiChatReceiver *receiver)
  {
    assert(receiver);
    pReceiver = receiver;
  }

  void Gui::addChatItem(const std::string &item)
  {
    CEGUI::Listbox *lbox = dynamic_cast<CEGUI::Listbox *>(CEGUI::WindowManager::getSingletonPtr()->getWindow("/ChatBox/List"));
    assert (lbox);
    CEGUI::ListboxTextItem *ltItem = new CEGUI::ListboxTextItem(CEGUI::String(item));
    lbox->addItem(ltItem);
    lbox->ensureItemIsVisible(ltItem);
  }

  bool Gui::chatMessageSent(const CEGUI::EventArgs &args)
  {
    if(pReceiver && pChatBox) {
      std::string message(pChatBox->getText().c_str());
      pChatBox->setText("");
      pReceiver->sendChatMessage(message);
      pChatBox->deactivate();
    }
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

    bool keysymProcessed, charProcessed;
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


  bool Gui::mouseMoved(const ap::ooinput::MouseMovedEvent &event)
  {
    assert(mSystem);
    return mSystem->injectMousePosition(static_cast<float>(event.xabs),
					static_cast<float>(event.yabs));
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
