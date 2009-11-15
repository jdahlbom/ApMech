#ifndef AP_GUIMAINMENURECEIVER_H
#define AP_GUIMAINMENURECEIVER_H

namespace ap {
  /**
   * Interface for allowing the GUI to call the implementing
   * class which controls the execution of Main Menu, without
   * requiring anything else of that class.
   */

  class GuiMainMenuReceiver {
  public:
    virtual ~GuiMainMenuReceiver() {} // Should not be called directly!
    virtual void receiveQuitGame()=0;
    virtual void receiveConfigureKeyboard()=0;
    virtual void receivePlayGame()=0;
  };
}

#endif
