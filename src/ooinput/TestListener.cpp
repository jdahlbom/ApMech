#include "TestListener.h"

#include <iostream>

#include "KeyEvent.h"

namespace Ap {

TestListener::TestListener()
{
    running=1;
}

TestListener::~TestListener() {}

bool TestListener::keyPressed(const KeyEvent &event)
{
    std::cout << event.unicode << " pressed." << std::endl;

    if(event.key == AP_K_ESCAPE)
        running=0;

    return false;
}

bool TestListener::keyReleased(const KeyEvent &event)
{
    std::cout << event.unicode << " released." << std::endl;
    return false;
}

} // namespace Ap
