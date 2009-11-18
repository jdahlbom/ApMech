#include "ActionKeyMap.h"

#include <cassert>
#include <map>
#include <string>
#include <utility>

#include <iostream>

#include "ooinput/KeyEvent.h"

namespace ap {

  ActionKeyMap::ActionKeyMap():
    currentKeyMap(std::map<ap::ooinput::KeySymbol, KeyMapTuple>()),
    actionNames(std::map<IngameAction, std::string>())
  {
    resetKeyMapToDefaults();
    fillActionNames();
  }

  ActionKeyMap::~ActionKeyMap() {}

  /**
   * Map a key to an action.
   *
   * @param KeyEvent that contains the keysym and unicode for the key.
   * @param name of the action to set.
   * @return True if inserted key is valid input key to map into action. False otherwise.
   */
  bool ActionKeyMap::setKeyForAction(const ap::ooinput::KeyEvent &e, std::string actionName)
  {
    IngameAction action = UNDEFINED;
    // check that the action exists
    std::map<IngameAction, std::string>::const_iterator nameIt;
    for(nameIt = actionNames.begin(); nameIt != actionNames.end(); ) {
      if( nameIt->second == actionName ) {
	action = nameIt->first;
	break;
      }
      ++nameIt;
    }
    if (UNDEFINED == action)
      return false;
   
    if ( !isValidKeySymToUse(e.key) )
      return false;

    // remove previous action mapped to this key    
    std::map<ap::ooinput::KeySymbol, KeyMapTuple>::iterator existingIt;
    existingIt = currentKeyMap.find(e.key);
    if ( currentKeyMap.end() != existingIt ) {
      currentKeyMap.erase(existingIt);
    }

    // remove previous key mapped to this action
    for ( existingIt=currentKeyMap.begin(); existingIt!=currentKeyMap.end(); ) {
      if (action == existingIt->second.action) {
	currentKeyMap.erase(existingIt);
	break;
      }
      ++existingIt;
    }

    // map this key to the action
    KeyMapTuple newEntry;
    newEntry.action = action;
    newEntry.unicode = e.unicode; // TODO: Should not use unicode in this class.
    newEntry.keyname = getProperKeyName(e.key, e.unicode);
    currentKeyMap.insert(std::make_pair(e.key, newEntry));
    std::cout << "new value: " << newEntry.keyname << std::endl;
    return true;
  }

  /**
   * There are some keys we want to reserve for hardcoded behavior.
   * ESCAPE is one of them.
   * @return True is inserted key is valid for use.
   */
  bool ActionKeyMap::isValidKeySymToUse(ap::ooinput::KeySymbol keysym) const {
    if (keysym == ap::ooinput::AP_K_ESCAPE)
      return false;
    return true;
  }

  std::string ActionKeyMap::getProperKeyName(ap::ooinput::KeySymbol key, ap::uint16 unicode) const {
    using namespace ap::ooinput;

    switch(key) {
    case AP_K_BACKSPACE: return "BACKSPACE";
    case AP_K_TAB: return "TAB";
    case AP_K_RETURN: return "RETURN";
    case AP_K_SPACE: return "SPACE";
    case AP_K_ESCAPE: return "ESC";
    case AP_K_RSHIFT: return "RIGHT SHIFT";
    case AP_K_LSHIFT: return "LEFT SHIFT";
    case AP_K_RCTRL: return "RIGHT CONTROL";
    case AP_K_LCTRL: return "LEFT CONTROL";
    case AP_K_RALT: return "RIGHT ALT";
    case AP_K_LALT: return "LEFT ALT";
      // TODO: These should use the unicode values instead of keysyms!
    case AP_K_a: return "A";
    case AP_K_b: return "B";
    case AP_K_c: return "C";
    case AP_K_d: return "D";
    case AP_K_e: return "E";
    case AP_K_f: return "F";
    case AP_K_g: return "G";
    case AP_K_h: return "H";
    case AP_K_i: return "I";
    case AP_K_j: return "J";
    case AP_K_k: return "K";
    case AP_K_l: return "L";
    case AP_K_m: return "M";
    case AP_K_n: return "N";
    case AP_K_o: return "O";
    case AP_K_p: return "P";
    case AP_K_q: return "Q";
    case AP_K_r: return "R";
    case AP_K_s: return "S";
    case AP_K_t: return "T";
    case AP_K_u: return "U";
    case AP_K_v: return "V";
    case AP_K_w: return "W";
    case AP_K_x: return "X";
    case AP_K_y: return "Y";
    case AP_K_z: return "Z";
    }
  }

  IngameAction ActionKeyMap::getActionForKey(ap::ooinput::KeySymbol keysym) const {
    std::map<ap::ooinput::KeySymbol, KeyMapTuple>::const_iterator it;
    it = currentKeyMap.find(keysym);
    if (currentKeyMap.end() == it) {
      return UNDEFINED;
    }
    // This key is in the map.
    return it->second.action;
  }

  std::string ActionKeyMap::getMappedKeyName(std::string actionName) const
  {
    std::map<IngameAction, std::string>::const_iterator actionIt;
    for (actionIt = actionNames.begin(); actionIt != actionNames.end();) {
      if ( actionName == actionIt->second ) {
	IngameAction thisAction = actionIt->first;

	return getKeyNameForAction(thisAction);
      } 
      ++actionIt;
    }

    assert(0==1); // This shouldn't be called with an invalid value.
    return "";
  }

  std::string ActionKeyMap::getKeyNameForAction(IngameAction action) const 
  {
    std::map<ap::ooinput::KeySymbol, KeyMapTuple>::const_iterator mapIt;
    for (mapIt=currentKeyMap.begin(); mapIt!=currentKeyMap.end(); ) {
      if (mapIt->second.action == action) {
	return mapIt->second.keyname;
      }
      ++mapIt;
    }
    return "";
  }

  /**
   * Used to loop through all available action names.
   * @param std::string The next action name will be stored in this variable.
   * @return True if value was available. False if at the end of the list.
   */
  bool ActionKeyMap::getEachActionName(std::string &name) const
  {
    if( actionNames.end()==eachIt) {
      eachIt = actionNames.begin();
      return false;
    }

    name = eachIt->second;
    ++eachIt;
    return true;
  }

  void ActionKeyMap::fillActionNames() {
    using std::make_pair;
    actionNames.clear();
    actionNames.insert(make_pair(ACCELERATE_FORWARD, "Move forward"));
    actionNames.insert(make_pair(ACCELERATE_BACKWARD, "Move backward"));
    actionNames.insert(make_pair(TURN_CLOCKWISE, "Turn right"));
    actionNames.insert(make_pair(TURN_COUNTERCLOCKWISE, "Turn left"));
    actionNames.insert(make_pair(FIRE_WEAPON, "Fire"));
    actionNames.insert(make_pair(START_MESSAGE, "Tell"));
    actionNames.insert(make_pair(TOGGLE_LIMBOMENU, "Limbo menu"));
    actionNames.insert(make_pair(TOGGLE_SCORES, "Show scores"));
    actionNames.insert(make_pair(QUIT_GAME, "Quit"));
    eachIt = actionNames.begin();
  }

  void ActionKeyMap::resetKeyMapToDefaults() {
    using namespace std;
    using namespace ap::ooinput;
    map<KeySymbol, KeyMapTuple>::iterator it;
    for(it=currentKeyMap.begin(); it!=currentKeyMap.end(); ++it) {
      currentKeyMap.erase(it);
    }

    KeyMapTuple temp;
    temp.unicode = 0;
    
    temp.keyname = getProperKeyName(AP_K_w,0);
    temp.action = ACCELERATE_FORWARD;
    currentKeyMap.insert(make_pair(AP_K_w, temp));
    
    temp.keyname=getProperKeyName(AP_K_s,0);
    temp.action = ACCELERATE_BACKWARD;
    currentKeyMap.insert(make_pair(AP_K_s, temp));

    temp.keyname=getProperKeyName(AP_K_a,0);
    temp.action = TURN_COUNTERCLOCKWISE;
    currentKeyMap.insert(make_pair(AP_K_a, temp));

    temp.keyname=getProperKeyName(AP_K_d,0);
    temp.action = TURN_CLOCKWISE;
    currentKeyMap.insert(make_pair(AP_K_d, temp));

    temp.keyname=getProperKeyName(AP_K_l,0);
    temp.action = TOGGLE_LIMBOMENU;
    currentKeyMap.insert(make_pair(AP_K_l, temp));

    temp.keyname=getProperKeyName(AP_K_t,0);
    temp.action = START_MESSAGE;
    currentKeyMap.insert(make_pair(AP_K_t, temp));

    temp.keyname=getProperKeyName(AP_K_SPACE,0);
    temp.action = FIRE_WEAPON;
    currentKeyMap.insert(make_pair(AP_K_SPACE, temp));

    temp.keyname=getProperKeyName(AP_K_ESCAPE,0);
    temp.action = QUIT_GAME;
    currentKeyMap.insert(make_pair(AP_K_ESCAPE, temp));

    temp.keyname=getProperKeyName(AP_K_TAB,0);
    temp.action = TOGGLE_SCORES;
    currentKeyMap.insert(make_pair(AP_K_TAB, temp));
  }
} // namespace ap
