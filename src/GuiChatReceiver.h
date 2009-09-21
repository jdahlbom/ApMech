#ifndef AP_GUICHATRECEIVER
#define AP_GUICHATRECEIVER

#include <string>
namespace ap {
  /**
   * Defines an interface for receiving messages sent to the chat box.
   */
  class GuiChatReceiver {
  public:
    void sendChatMessage(const std::string &message);
  };
} // namespace ap

#endif
