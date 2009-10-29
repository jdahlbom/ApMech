#ifndef WIN32
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif

#include <iostream>

#include "SDLInputSystem.h"
#include "KeyEvent.h"
#include "MouseClickedEvent.h"
#include "MouseMovedEvent.h"

namespace ap {
namespace ooinput {

SDLInputSystem::SDLInputSystem()
{
    SDL_EnableUNICODE(1);
}
SDLInputSystem::~SDLInputSystem() {}

void SDLInputSystem::capture()
{
    // Poll for events
    while(SDL_PollEvent(&sdlEvent))
    {
      // Process
      switch(sdlEvent.type) {

	// Keypress events
      case SDL_KEYDOWN:
	fireKeyPressedEvent( SDLInputSystem::createKeyEvent(sdlEvent.key.keysym, AP_E_KEYDOWN) );
	break;
      case SDL_KEYUP:
	fireKeyReleasedEvent( SDLInputSystem::createKeyEvent(sdlEvent.key.keysym, AP_E_KEYUP) );
	break;		

	//Mouse motion events
      case SDL_MOUSEMOTION:
	fireMouseMovedEvent( SDLInputSystem::createMouseMovedEvent(sdlEvent.motion) );
	break;
	
	// Mouse click events
      case SDL_MOUSEBUTTONDOWN:
	fireMousePressedEvent( SDLInputSystem::createMouseClickedEvent(sdlEvent.button));
	break;
      case SDL_MOUSEBUTTONUP:
	fireMouseReleasedEvent( SDLInputSystem::createMouseClickedEvent(sdlEvent.button));
	break;

      default:
	break;
      }
    }
}

KeyEvent SDLInputSystem::createKeyEvent(SDL_keysym &keysym, KeyEventType type)
{
    KeyEvent event;
    event.type = type;
    event.unicode = keysym.unicode;
    event.key = mapKey(keysym.sym);

    return event;
}

KeySymbol SDLInputSystem::mapKey(SDLKey key)
{
    switch(key) {
    case SDLK_BACKSPACE: return AP_K_BACKSPACE;
    case SDLK_TAB: return AP_K_TAB;
    case SDLK_CLEAR: return AP_K_CLEAR;
    case SDLK_RETURN: return AP_K_RETURN;
    case SDLK_PAUSE: return AP_K_PAUSE;
    case SDLK_ESCAPE: return AP_K_ESCAPE;
    case SDLK_SPACE: return AP_K_SPACE;
    case SDLK_EXCLAIM: return AP_K_EXCLAIM;
    case SDLK_QUOTEDBL: return AP_K_QUOTEDBL;
    case SDLK_HASH: return AP_K_HASH;
    case SDLK_DOLLAR: return AP_K_DOLLAR;
    case SDLK_AMPERSAND: return AP_K_AMPERSAND;
    case SDLK_QUOTE: return AP_K_QUOTE;
    case SDLK_LEFTPAREN: return AP_K_LEFTPAREN;
    case SDLK_RIGHTPAREN: return AP_K_RIGHTPAREN;
    case SDLK_ASTERISK: return AP_K_ASTERISK;
    case SDLK_PLUS: return AP_K_PLUS;
    case SDLK_COMMA: return AP_K_COMMA;
    case SDLK_MINUS: return AP_K_MINUS;
    case SDLK_PERIOD: return AP_K_PERIOD;
    case SDLK_SLASH: return AP_K_SLASH;
    case SDLK_0: return AP_K_0;
    case SDLK_1: return AP_K_1;
    case SDLK_2: return AP_K_2;
    case SDLK_3: return AP_K_3;
    case SDLK_4: return AP_K_4;
    case SDLK_5: return AP_K_5;
    case SDLK_6: return AP_K_6;
    case SDLK_7: return AP_K_7;
    case SDLK_8: return AP_K_8;
    case SDLK_9: return AP_K_9;
    case SDLK_COLON: return AP_K_COLON;
    case SDLK_SEMICOLON: return AP_K_SEMICOLON;
    case SDLK_LESS: return AP_K_LESS;
    case SDLK_EQUALS: return AP_K_EQUALS;
    case SDLK_GREATER: return AP_K_GREATER;
    case SDLK_QUESTION: return AP_K_QUESTION;
    case SDLK_AT: return AP_K_AT;
    /*
        Skip uppercase letters
    */
    case SDLK_LEFTBRACKET: return AP_K_LEFTBRACKET;
    case SDLK_BACKSLASH: return AP_K_BACKSLASH;
    case SDLK_RIGHTBRACKET: return AP_K_RIGHTBRACKET;
    case SDLK_CARET: return AP_K_CARET;
    case SDLK_UNDERSCORE: return AP_K_UNDERSCORE;
    case SDLK_BACKQUOTE: return AP_K_BACKQUOTE;
    case SDLK_a: return AP_K_a;
    case SDLK_b: return AP_K_b;
    case SDLK_c: return AP_K_c;
    case SDLK_d: return AP_K_d;
    case SDLK_e: return AP_K_e;
    case SDLK_f: return AP_K_f;
    case SDLK_g: return AP_K_g;
    case SDLK_h: return AP_K_h;
    case SDLK_i: return AP_K_i;
    case SDLK_j: return AP_K_j;
    case SDLK_k: return AP_K_k;
    case SDLK_l: return AP_K_l;
    case SDLK_m: return AP_K_m;
    case SDLK_n: return AP_K_n;
    case SDLK_o: return AP_K_o;
    case SDLK_p: return AP_K_p;
    case SDLK_q: return AP_K_q;
    case SDLK_r: return AP_K_r;
    case SDLK_s: return AP_K_s;
    case SDLK_t: return AP_K_t;
    case SDLK_u: return AP_K_u;
    case SDLK_v: return AP_K_v;
    case SDLK_w: return AP_K_w;
    case SDLK_x: return AP_K_x;
    case SDLK_y: return AP_K_y;
    case SDLK_z: return AP_K_z;
    case SDLK_DELETE: return AP_K_DELETE;
    /* End of ASCII mapped keysyms */

    /* International keyboard syms */
    case SDLK_WORLD_0: return AP_K_WORLD_0;        /* 0xA0 */
    case SDLK_WORLD_1: return AP_K_WORLD_1;
    case SDLK_WORLD_2: return AP_K_WORLD_2;
    case SDLK_WORLD_3: return AP_K_WORLD_3;
    case SDLK_WORLD_4: return AP_K_WORLD_4;
    case SDLK_WORLD_5: return AP_K_WORLD_5;
    case SDLK_WORLD_6: return AP_K_WORLD_6;
    case SDLK_WORLD_7: return AP_K_WORLD_7;
    case SDLK_WORLD_8: return AP_K_WORLD_8;
    case SDLK_WORLD_9: return AP_K_WORLD_9;
    case SDLK_WORLD_10: return AP_K_WORLD_10;
    case SDLK_WORLD_11: return AP_K_WORLD_11;
    case SDLK_WORLD_12: return AP_K_WORLD_12;
    case SDLK_WORLD_13: return AP_K_WORLD_13;
    case SDLK_WORLD_14: return AP_K_WORLD_14;
    case SDLK_WORLD_15: return AP_K_WORLD_15;
    case SDLK_WORLD_16: return AP_K_WORLD_16;
    case SDLK_WORLD_17: return AP_K_WORLD_17;
    case SDLK_WORLD_18: return AP_K_WORLD_18;
    case SDLK_WORLD_19: return AP_K_WORLD_19;
    case SDLK_WORLD_20: return AP_K_WORLD_20;
    case SDLK_WORLD_21: return AP_K_WORLD_21;
    case SDLK_WORLD_22: return AP_K_WORLD_22;
    case SDLK_WORLD_23: return AP_K_WORLD_23;
    case SDLK_WORLD_24: return AP_K_WORLD_24;
    case SDLK_WORLD_25: return AP_K_WORLD_25;
    case SDLK_WORLD_26: return AP_K_WORLD_26;
    case SDLK_WORLD_27: return AP_K_WORLD_27;
    case SDLK_WORLD_28: return AP_K_WORLD_28;
    case SDLK_WORLD_29: return AP_K_WORLD_29;
    case SDLK_WORLD_30: return AP_K_WORLD_30;
    case SDLK_WORLD_31: return AP_K_WORLD_31;
    case SDLK_WORLD_32: return AP_K_WORLD_32;
    case SDLK_WORLD_33: return AP_K_WORLD_33;
    case SDLK_WORLD_34: return AP_K_WORLD_34;
    case SDLK_WORLD_35: return AP_K_WORLD_35;
    case SDLK_WORLD_36: return AP_K_WORLD_36;
    case SDLK_WORLD_37: return AP_K_WORLD_37;
    case SDLK_WORLD_38: return AP_K_WORLD_38;
    case SDLK_WORLD_39: return AP_K_WORLD_39;
    case SDLK_WORLD_40: return AP_K_WORLD_40;
    case SDLK_WORLD_41: return AP_K_WORLD_41;
    case SDLK_WORLD_42: return AP_K_WORLD_42;
    case SDLK_WORLD_43: return AP_K_WORLD_43;
    case SDLK_WORLD_44: return AP_K_WORLD_44;
    case SDLK_WORLD_45: return AP_K_WORLD_45;
    case SDLK_WORLD_46: return AP_K_WORLD_46;
    case SDLK_WORLD_47: return AP_K_WORLD_47;
    case SDLK_WORLD_48: return AP_K_WORLD_48;
    case SDLK_WORLD_49: return AP_K_WORLD_49;
    case SDLK_WORLD_50: return AP_K_WORLD_50;
    case SDLK_WORLD_51: return AP_K_WORLD_51;
    case SDLK_WORLD_52: return AP_K_WORLD_52;
    case SDLK_WORLD_53: return AP_K_WORLD_53;
    case SDLK_WORLD_54: return AP_K_WORLD_54;
    case SDLK_WORLD_55: return AP_K_WORLD_55;
    case SDLK_WORLD_56: return AP_K_WORLD_56;
    case SDLK_WORLD_57: return AP_K_WORLD_57;
    case SDLK_WORLD_58: return AP_K_WORLD_58;
    case SDLK_WORLD_59: return AP_K_WORLD_59;
    case SDLK_WORLD_60: return AP_K_WORLD_60;
    case SDLK_WORLD_61: return AP_K_WORLD_61;
    case SDLK_WORLD_62: return AP_K_WORLD_62;
    case SDLK_WORLD_63: return AP_K_WORLD_63;
    case SDLK_WORLD_64: return AP_K_WORLD_64;
    case SDLK_WORLD_65: return AP_K_WORLD_65;
    case SDLK_WORLD_66: return AP_K_WORLD_66;
    case SDLK_WORLD_67: return AP_K_WORLD_67;
    case SDLK_WORLD_68: return AP_K_WORLD_68;
    case SDLK_WORLD_69: return AP_K_WORLD_69;
    case SDLK_WORLD_70: return AP_K_WORLD_70;
    case SDLK_WORLD_71: return AP_K_WORLD_71;
    case SDLK_WORLD_72: return AP_K_WORLD_72;
    case SDLK_WORLD_73: return AP_K_WORLD_73;
    case SDLK_WORLD_74: return AP_K_WORLD_74;
    case SDLK_WORLD_75: return AP_K_WORLD_75;
    case SDLK_WORLD_76: return AP_K_WORLD_76;
    case SDLK_WORLD_77: return AP_K_WORLD_77;
    case SDLK_WORLD_78: return AP_K_WORLD_78;
    case SDLK_WORLD_79: return AP_K_WORLD_79;
    case SDLK_WORLD_80: return AP_K_WORLD_80;
    case SDLK_WORLD_81: return AP_K_WORLD_81;
    case SDLK_WORLD_82: return AP_K_WORLD_82;
    case SDLK_WORLD_83: return AP_K_WORLD_83;
    case SDLK_WORLD_84: return AP_K_WORLD_84;
    case SDLK_WORLD_85: return AP_K_WORLD_85;
    case SDLK_WORLD_86: return AP_K_WORLD_86;
    case SDLK_WORLD_87: return AP_K_WORLD_87;
    case SDLK_WORLD_88: return AP_K_WORLD_88;
    case SDLK_WORLD_89: return AP_K_WORLD_89;
    case SDLK_WORLD_90: return AP_K_WORLD_90;
    case SDLK_WORLD_91: return AP_K_WORLD_91;
    case SDLK_WORLD_92: return AP_K_WORLD_92;
    case SDLK_WORLD_93: return AP_K_WORLD_93;
    case SDLK_WORLD_94: return AP_K_WORLD_94;
    case SDLK_WORLD_95: return AP_K_WORLD_95;        /* 0xFF */

    /* Numeric keypad */
    case SDLK_KP0: return AP_K_KP0;
    case SDLK_KP1: return AP_K_KP1;
    case SDLK_KP2: return AP_K_KP2;
    case SDLK_KP3: return AP_K_KP3;
    case SDLK_KP4: return AP_K_KP4;
    case SDLK_KP5: return AP_K_KP5;
    case SDLK_KP6: return AP_K_KP6;
    case SDLK_KP7: return AP_K_KP7;
    case SDLK_KP8: return AP_K_KP8;
    case SDLK_KP9: return AP_K_KP9;
    case SDLK_KP_PERIOD: return AP_K_KP_PERIOD;
    case SDLK_KP_DIVIDE: return AP_K_KP_DIVIDE;
    case SDLK_KP_MULTIPLY: return AP_K_KP_MULTIPLY;
    case SDLK_KP_MINUS: return AP_K_KP_MINUS;
    case SDLK_KP_PLUS: return AP_K_KP_PLUS;
    case SDLK_KP_ENTER: return AP_K_KP_ENTER;
    case SDLK_KP_EQUALS: return AP_K_KP_EQUALS;

    /* Arrows + Home/End pad */
    case SDLK_UP: return AP_K_UP;
    case SDLK_DOWN: return AP_K_DOWN;
    case SDLK_RIGHT: return AP_K_RIGHT;
    case SDLK_LEFT: return AP_K_LEFT;
    case SDLK_INSERT: return AP_K_INSERT;
    case SDLK_HOME: return AP_K_HOME;
    case SDLK_END: return AP_K_END;
    case SDLK_PAGEUP: return AP_K_PAGEUP;
    case SDLK_PAGEDOWN: return AP_K_PAGEDOWN;

    /* Function keys */
    case SDLK_F1: return AP_K_F1;
    case SDLK_F2: return AP_K_F2;
    case SDLK_F3: return AP_K_F3;
    case SDLK_F4: return AP_K_F4;
    case SDLK_F5: return AP_K_F5;
    case SDLK_F6: return AP_K_F6;
    case SDLK_F7: return AP_K_F7;
    case SDLK_F8: return AP_K_F8;
    case SDLK_F9: return AP_K_F9;
    case SDLK_F10: return AP_K_F10;
    case SDLK_F11: return AP_K_F11;
    case SDLK_F12: return AP_K_F12;
    case SDLK_F13: return AP_K_F13;
    case SDLK_F14: return AP_K_F14;
    case SDLK_F15: return AP_K_F15;

    /* Key state modifier keys */
    case SDLK_NUMLOCK: return AP_K_NUMLOCK;
    case SDLK_CAPSLOCK: return AP_K_CAPSLOCK;
    case SDLK_SCROLLOCK: return AP_K_SCROLLOCK;
    case SDLK_RSHIFT: return AP_K_RSHIFT;
    case SDLK_LSHIFT: return AP_K_LSHIFT;
    case SDLK_RCTRL: return AP_K_RCTRL;
    case SDLK_LCTRL: return AP_K_LCTRL;
    case SDLK_RALT: return AP_K_RALT;
    case SDLK_LALT: return AP_K_LALT;
    case SDLK_RMETA: return AP_K_RMETA;
    case SDLK_LMETA: return AP_K_LMETA;
    case SDLK_MODE: return AP_K_MODE;        /* "Alt Gr" key */


    /* Miscellaneous function keys */
    case SDLK_HELP: return AP_K_HELP;
    case SDLK_PRINT: return AP_K_PRINT;
    case SDLK_SYSREQ: return AP_K_SYSREQ;
    case SDLK_BREAK: return AP_K_BREAK;
    case SDLK_MENU: return AP_K_MENU;
    case SDLK_POWER: return AP_K_POWER;        /* Power Macintosh power key */
    case SDLK_EURO: return AP_K_EURO;        /* Some european keyboards */
    case SDLK_UNDO: return AP_K_UNDO;        /* Atari keyboard has Undo */
    default:          return AP_K_UNDEF;
    }
}

MouseClickedEvent SDLInputSystem::createMouseClickedEvent(SDL_MouseButtonEvent &event) 
{
  MouseClickedEvent mouseEvent;
  switch(event.type) {
  case SDL_MOUSEBUTTONDOWN: 
    mouseEvent.type=AP_E_MOUSEBUTTONDOWN; 
    break;
  case SDL_MOUSEBUTTONUP:   
    mouseEvent.type=AP_E_MOUSEBUTTONUP;   
    break;
  default: 
    mouseEvent.type=AP_E_UNDEFINED; 
  }

  switch(event.button) {
  case SDL_BUTTON_LEFT:
    mouseEvent.button=AP_B_LEFT;
    break;
  case SDL_BUTTON_RIGHT:
    mouseEvent.button=AP_B_RIGHT;
    break;
  case SDL_BUTTON_MIDDLE:
    mouseEvent.button=AP_B_MIDDLE;
    break;
  case SDL_BUTTON_WHEELDOWN:
    mouseEvent.button=AP_B_WHEELDOWN;
    break;
  case SDL_BUTTON_WHEELUP:
    mouseEvent.button=AP_B_WHEELUP;
    break;
  default:
    mouseEvent.button=AP_B_UNDEFINED;
  }

  mouseEvent.x = event.x;
  mouseEvent.y = event.y;

  return mouseEvent;
}

MouseMovedEvent SDLInputSystem::createMouseMovedEvent(SDL_MouseMotionEvent &event)
{
  MouseMovedEvent mouseEvent;
  mouseEvent.state=event.state;
  mouseEvent.xabs=event.x;
  mouseEvent.yabs=event.y;
  mouseEvent.xrel=event.xrel;
  mouseEvent.yrel=event.yrel;

  return mouseEvent;
}

} //namespace ooinput
} //namespace ap
