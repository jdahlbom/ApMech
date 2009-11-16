#include "ActionKeyMap.h"

#include <cassert>
#include <map>
#include <string>
#include <utility>

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
    assert(1==0); // This shouldn't be called with an invalid value.
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
    
    temp.keyname = "w";
    temp.action = ACCELERATE_FORWARD;
    currentKeyMap.insert(make_pair(AP_K_w, temp));
    
    temp.keyname="s";
    temp.action = ACCELERATE_BACKWARD;
    currentKeyMap.insert(make_pair(AP_K_s, temp));

    temp.keyname="a";
    temp.action = TURN_COUNTERCLOCKWISE;
    currentKeyMap.insert(make_pair(AP_K_a, temp));

    temp.keyname="d";
    temp.action = TURN_CLOCKWISE;
    currentKeyMap.insert(make_pair(AP_K_d, temp));

    temp.keyname="l";
    temp.action = TOGGLE_LIMBOMENU;
    currentKeyMap.insert(make_pair(AP_K_l, temp));

    temp.keyname="t";
    temp.action = START_MESSAGE;
    currentKeyMap.insert(make_pair(AP_K_t, temp));

    temp.keyname="SPACE";
    temp.action = FIRE_WEAPON;
    currentKeyMap.insert(make_pair(AP_K_SPACE, temp));

    temp.keyname="ESC";
    temp.action = QUIT_GAME;
    currentKeyMap.insert(make_pair(AP_K_ESCAPE, temp));

    temp.keyname="TAB";
    temp.action = TOGGLE_SCORES;
    currentKeyMap.insert(make_pair(AP_K_TAB, temp));
  }
} // namespace ap
