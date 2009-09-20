#ifndef AP_GUI
#define AP_GUI

#include <CEGUI.h>

namespace ap {

class Gui
{
 public:
  Gui(CEGUI::Renderer *renderer);
  ~Gui();

  void setupChatBox();

 private:
  CEGUI::System *mSystem;
  CEGUI::Window *pRoot;
};

} // namespace ap
#endif
