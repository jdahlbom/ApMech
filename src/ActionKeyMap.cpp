#include "ActionKeyMap.h"

#include <map>
#include <string>
#include <utility>

#include "ooinput/KeyEvent.h"

namespace ap {

  ActionKeyMap::ActionKeyMap():
    currentKeyMap(std::map<ap::ooinput::KeySymbol, KeyMapTuple>())
  {
    resetKeyMapToDefaults();
  }

  ActionKeyMap::~ActionKeyMap() {}

  IngameAction ActionKeyMap::getActionForKey(ap::ooinput::KeySymbol keysym) {
    std::map<ap::ooinput::KeySymbol, KeyMapTuple>::const_iterator it;
    it = currentKeyMap.find(keysym);
    if (currentKeyMap.end() == it) {
      return UNDEFINED;
    }
    // This key is in the map.
    return it->second.action;
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
