#include "apeventhandler.h"

#include <ostream.h>

#include <CEGUI/CEGUI.h>


ApEventHandler::ApEventHandler(GameEngine *engine): engine_(engine) {}
ApEventHandler::~ApEventHandler() {}

bool ApEventHandler::quit(const CEGUI::EventArgs &arg)
{
    ostream::cout << "ApEventHandler::quit\n";
    engine_->keepRendering = false;
    return true;
}
