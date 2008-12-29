#include "apeventhandler.h"

#include <iostream>

#include <CEGUI/CEGUI.h>


ApEventHandler::ApEventHandler(GameEngine *engine): engine_(engine) {}
ApEventHandler::~ApEventHandler() {}

bool ApEventHandler::quit(const CEGUI::EventArgs &arg)
{
    std::cout << "ApEventHandler::quit"<< std::endl;
    engine_->keepRendering = false;
    return true;
}
