#ifndef AP_ACTIONKEYMAP_H
#define AP_ACTIONKEYMAP_H

#include <map>
#include <string>

#include "ooinput/KeyEvent.h"

namespace ap {
  typedef enum {
    UNDEFINED             = 0,
    // [Movement]
    ACCELERATE_FORWARD    = 1,
    ACCELERATE_BACKWARD   = 2,
    TURN_CLOCKWISE        = 3,
    TURN_COUNTERCLOCKWISE = 4,
    // [Weapons]
    FIRE_WEAPON           = 64,
    // [Miscallaneous]
    START_MESSAGE         = 128,
    TOGGLE_LIMBOMENU      = 129,
    QUIT_GAME             = 130,
    TOGGLE_SCORES         = 131
  } IngameAction;

  // TODO:: This should be an inner class to ActionKeyMap, not used elsewhere.
  typedef struct{
    ap::uint16  unicode; // Unicode char representing this keysym. User-local.
    std::string keyname; // Alternative to unicode - used for special keys.
    IngameAction action; // Action mapped to this key.
  } KeyMapTuple;

  /**
   * ActionKeyMap represents the mapping from keysymbols to in-game actions.
   * Class also handles storing and retrieval of different keymap configurations
   **/
  class ActionKeyMap {
  public:
    ActionKeyMap();
    ~ActionKeyMap();
    // getActionNames(); // Miten tämä tehdään??
    IngameAction getActionForKey(ap::ooinput::KeySymbol keysym) const;
    std::string getMappedKeyName(std::string actionName) const;
    bool getEachActionName(std::string &name) const;
    
  private:
    std::map<ap::ooinput::KeySymbol, KeyMapTuple> currentKeyMap;
    std::map<IngameAction, std::string> actionNames;
    mutable std::map<IngameAction, std::string>::const_iterator eachIt;

    // TODO: this is a 1-to-1 mapping between key and action
    std::string getKeyNameForAction(IngameAction action) const;
    void fillActionNames();
    void resetKeyMapToDefaults();
  }; // class ActionKeyMap
} // namespace ap

#endif
