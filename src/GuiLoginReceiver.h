#ifndef AP_GUILOGINRECEIVER_H
#define AP_GUILOGINRECEIVER_H

#include <string>

namespace ap {
  class GuiLoginReceiver {
  public:
    virtual void attemptLogin(const std::string &ipAddress, const std::string &playerName) =0;
    virtual void requestQuit() =0;
  };
} // namespace ap

#endif
