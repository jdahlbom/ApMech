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
	SceneNode *trgnode = mEngine->mPlayer->getSceneNode();
	Matrix3 axes = mEngine->mPlayer->getSceneNode()->getLocalAxes();

	switch((int)forward) {
		case 1:
			trgnode->translate(axes, Vector3(1,0,0)); //forward in local object space
			break;
		case -1:
			trgnode->translate(axes, Vector3(-1,0,0)); //back in local object space
			break;
		default:
			break;
	}
	switch((int)right) {
		case -1:
			trgnode->yaw(Radian(Degree(5)));
			break;
		case 1:
			trgnode->yaw(Radian(Degree(-5)));
			break;
		default:
			break;
	}

	mEngine->mPlayer->setLocation(trgnode->getPosition());

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
