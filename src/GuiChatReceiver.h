#ifndef AP_GUICHATRECEIVER
#define AP_GUICHATRECEIVER

#include <string>
namespace ap {
  /**
   * Defines an interface for receiving messages sent to the chat box.
   */
  class GuiChatReceiver {
  public:
    virtual void sendChatMessage(const std::string &message) = 0;
  };
} // namespace ap

#endif
