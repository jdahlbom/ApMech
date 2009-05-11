#include "KeyboardListener.h"

namespace Ap {

class TestListener: public KeyboardListener
{
    public:
    bool running;

    TestListener();
    ~TestListener();
    bool keyPressed(const KeyEvent &event);
    bool keyReleased(const KeyEvent &event);
};

} // namespace Ap
