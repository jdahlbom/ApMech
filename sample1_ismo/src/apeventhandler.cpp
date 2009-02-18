#include "apeventhandler.h"

#include "gameobject.h"

ApEventHandler::ApEventHandler(GameEngine *engine): mEngine(engine)
{
    mApGui = 0;

    mControlForward = 0.0f;
    mControlLeft = 0.0f;

    mMovementSpeed = 100;
    mTurningSpeed = 50;
}

ApEventHandler::~ApEventHandler() {}

bool ApEventHandler::quit()
{
    mEngine->keepRendering = false;
    return true;
}

bool ApEventHandler::moveUp()
{
    mControlForward = 1.0f;
    return true;
}

bool ApEventHandler::moveDown()
{
    mControlForward = -0.9f;
    return true;
}

bool ApEventHandler::moveRight()
{
    mControlLeft = -1.0f;
    return true;
}

bool ApEventHandler::moveLeft()
{
    mControlLeft = 1.0f;
    return true;
}

void ApEventHandler::move(float msTimeSinceLast)
{
    if ( (0 != mControlForward) || (0 != mControlLeft) ) {
        SceneNode *trgnode = mEngine->mPlayer->getSceneNode();
        Matrix3 axes = mEngine->mPlayer->getSceneNode()->getLocalAxes();

        if ( 0 != mControlForward ) {
            trgnode->translate(axes, Vector3( mControlForward * mMovementSpeed * msTimeSinceLast,0,0));
        }

        if ( 0 != mControlLeft ) {
            trgnode->yaw(Radian(Degree(mControlLeft * mTurningSpeed * msTimeSinceLast)));
        }

        mEngine->mPlayer->setLocation(trgnode->getPosition());

        markAsChanged();
    }
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
