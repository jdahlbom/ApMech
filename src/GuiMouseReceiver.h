#ifndef AP_GUIMOUSERECEIVER_H
#define AP_GUIMOUSERECEIVER_H

namespace ap {

  /**
   * Interface for allowing the GUI to call the implementing
   * class which needs to know about mouse position. Later on, maybe refactor
   * this to pass complete mouse knowledge.
   */

  /* this could be also someplace else */
  enum targetState {
    TARGET_NOT_AVAILABLE,
    TARGET_WITHIN_TORSO_TURN_ANGLE,
    TARGET_LINED_WITH_TORSO
  };

  class GuiMouseReceiver {
  public:
    virtual ~GuiMouseReceiver() {} // Should not be called directly!
    virtual void receiveMousePosition(int x, int y)=0;
  };
}

#endif
