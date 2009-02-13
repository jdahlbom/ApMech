#include "apeventhandler.h"

#include "gameobject.h"

ApEventHandler::ApEventHandler(GameEngine *engine): mEngine(engine)
{
    mApGui = 0;
}

ApEventHandler::~ApEventHandler() {}

bool ApEventHandler::quit()
{
    mEngine->keepRendering = false;
    return true;
}

bool ApEventHandler::moveUp()
{
    move(1.0f, 0.0f);
    return true;
}

bool ApEventHandler::moveDown()
{
    move(-1.0f, 0.0f);
    return true;
}

bool ApEventHandler::moveRight()
{
    move(0.0f, 1.0f);
    return true;
}

bool ApEventHandler::moveLeft()
{
    move(0.0f, -1.0f);
    return true;
}

void ApEventHandler::move(float forward, float right)
{
    Vector3 playerLocation = mEngine->mPlayer->getLocation();
    mEngine->mPlayer->setLocation(playerLocation + Vector3(Real(forward), Real(0.0), Real(right)));
    markAsChanged();
}

void ApEventHandler::markAsChanged()
{
    // FIXME:: Uudelleennimeä funktio -tuo tekee mitä tätä varten tarvitaan.
    mEngine->processKbEvent(0);
}

void ApEventHandler::toggleMainMenu()
{
    if ( 0 == mApGui )
    {
        std::cout << "ApGui pointer not set, doing nothing." << std::endl;
        return;
    }

    mApGui->toggleMainMenu();
}

void ApEventHandler::setGui(ApGui *guiPointer)
{
    mApGui = guiPointer;
}
