#include <Ogre.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <Carbon/Carbon.h>
#endif

#include "PlayState.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE

// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the current working directory
// for locating your configuration files and resources.
std::string macBundlePath()
{
    char path[1024];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    assert(mainBundle);

    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    assert(mainBundleURL);

    CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
    assert(cfStringRef);

    CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

    CFRelease(mainBundleURL);
    CFRelease(cfStringRef);

    return std::string(path);
}
#endif

namespace Ap {

PlayState::PlayState( GameStateManager *gameStateManager,
                      Ogre::SceneManager *sceneManager) :
                      pSceneManager(sceneManager),
                      currentObjectIndex(0)
{
    initStateManager(gameStateManager);

    pRoot = Ogre::Root::getSingletonPtr();
    createGUIWindow();
}
PlayState::~PlayState() {}

void PlayState::enter( void ) {
    // Create the terrain

    Ogre::Vector3 terrainCenter = Ogre::Vector3(750, 0, 750);

    #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        pSceneManager->setWorldGeometry(macBundlePath() + "/Contents/Resources/Media/terrain.cfg");
    #else
        pSceneManager->setWorldGeometry("terrain.cfg");
    #endif

    mCamera           = pSceneManager->createCamera( "PlayerCamera" );
    // REQUIRES for the RenderTarget named "ApMech" to exist..
    Ogre::RenderTarget *renderWindow = pRoot->getRenderTarget("ApMech");
    renderWindow->addViewport( mCamera )->setBackgroundColour(Ogre::ColourValue(0.4f,0.0f,0.4f));

    // Create the player character
    Ogre::Entity *myRobot = pSceneManager->createEntity("PlayerMech", "robot.mesh");
    myRobot->setVisible(true);
    myRobot->setCastShadows(true);
    Ogre::SceneNode *rootNode = pSceneManager->getRootSceneNode();
    mRobotNode = rootNode->createChildSceneNode("Node/MyRobot");
    mRobotNode->setPosition(terrainCenter);
    mRobotNode->attachObject(myRobot);

    mObject = new MovingObject();
    mObject->setPosition(terrainCenter);
    mObject->setOwnerNode(mRobotNode);
    mObject->setWorldBoundaries(1500.0f,0.0f,0.0f,1500.0f);
    mObject->setMaxSpeed(25.0f);

    objectsMap.insert(std::pair<unsigned int, MovingObject *>(++currentObjectIndex, mObject));

    // Attach a camera to the player model
    Ogre::SceneNode *cameraNode = mRobotNode->createChildSceneNode("Node/MyCamera");
    cameraNode->setPosition(Ogre::Vector3(0, 250,10));
    cameraNode->lookAt(mRobotNode->getPosition(), Ogre::Node::TS_WORLD);
    cameraNode->attachObject(mCamera);
    mCamera->setNearClipDistance(5);

    // Create lighting
    pSceneManager->setAmbientLight(Ogre::ColourValue(1,1,1));
    Ogre::Light *myLight= pSceneManager->createLight("MyLight");
    myLight->setType(Ogre::Light::LT_DIRECTIONAL);
    myLight->setDiffuseColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    myLight->setSpecularColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    myLight->setDirection(Ogre::Vector3(0,-1,1));
    Ogre::Light *myLight2= pSceneManager->createLight("MyLight2");
    myLight2->setType(Ogre::Light::LT_DIRECTIONAL);
    myLight2->setDiffuseColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    myLight2->setSpecularColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    myLight2->setDirection(Ogre::Vector3(1,0,-1));

    // Show GUI
    mStateOverlay->show();
    mStateOverlay->activate();

    std::cout << "Entering PlayState" << std::endl;
}

void PlayState::exit( void ) {
    mStateOverlay->deactivate();
    mStateOverlay->hide();
    std::cout << "Exiting PlayState" << std::endl;

    // Delete the moving objects
    std::map<unsigned int, MovingObject*>::iterator it = objectsMap.begin();
    for( ; it != objectsMap.end(); ++it) {
        delete(it->second);
    }
    objectsMap.erase( objectsMap.begin(), objectsMap.end() );
}

void PlayState::pause( void ) {
}

void PlayState::resume( void ) {
}

void PlayState::update( unsigned long lTimeElapsed ) {
    for( std::map<unsigned int, MovingObject*>::iterator it = objectsMap.begin(); it!=objectsMap.end(); ++it) {
        it->second->update(lTimeElapsed);
    }

}

//-----------------------------------------------------------------------------

void PlayState::createGUIWindow()
{
    CEGUI::System *ceguiSystem= CEGUI::System::getSingletonPtr();
    CEGUI::Window *ceguiRoot = ceguiSystem->getGUISheet();
    CEGUI::WindowManager *winMgr = CEGUI::WindowManager::getSingletonPtr();
    CEGUI::UVector2 buttonSize = CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0));

    // Menu main page

    mStateOverlay = winMgr->createWindow(
            (CEGUI::utf8*) "TaharezLook/Button",
            (CEGUI::utf8*) "root/playState/Button",
            (CEGUI::utf8*) "");

    mStateOverlay->setAlpha(0.5f);
    mStateOverlay->setText("Play");
    mStateOverlay->setSize(buttonSize);
    mStateOverlay->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.6, 0)));

    mStateOverlay->deactivate();
    mStateOverlay->hide();
    ceguiRoot->addChildWindow(mStateOverlay);

    std::cout << "PlayState::createGUIWindow finished" << std::endl;
}

bool PlayState::keyPressed( const Ap::KeyEvent &e ) {
    switch( e.key ) {
        case AP_K_ESCAPE:
            std::cout << "Escape pressed, quitting." << std::endl;
            this->requestShutdown();
            break;
        case AP_K_a:
            mObject->addClockwiseTurningSpeed(5);
            break;
        case AP_K_w:
            mObject->addForwardAcceleration(15);
            break;
        case AP_K_s:
            mObject->addForwardAcceleration(-9);
            break;
        case AP_K_d:
            mObject->addClockwiseTurningSpeed(-5);
            break;
        case AP_K_SPACE:
            fireGun();
            break;
        default:
            std::cout << e.unicode << " pressed, not doing anything." << std::endl;
            break;
    }
  return false;
}

bool PlayState::keyReleased( const Ap::KeyEvent &e ) {
    switch( e.key ) {
        case AP_K_a:
            mObject->addClockwiseTurningSpeed(-5);
            break;
        case AP_K_w:
            mObject->addForwardAcceleration(-15);
            break;
        case AP_K_s:
            mObject->addForwardAcceleration(9);
            break;
        case AP_K_d:
            mObject->addClockwiseTurningSpeed(5);
            break;
        default:
            break;
    }

  return false;
}


void PlayState::fireGun()
{
    std::stringstream ss;
    ss << ++currentObjectIndex;

    Ogre::Entity* bullet = pSceneManager->createEntity( ss.str(), "ninja.mesh" );
    Ogre::SceneNode* rootNode = pSceneManager->getRootSceneNode();

    ss << "-scenenode";
    Ogre::SceneNode* bulletNode = rootNode->createChildSceneNode( ss.str() );
    Ogre::Real realSmall = Ogre::Real(0.4f);
    bulletNode->scale( realSmall, realSmall, realSmall);
    bulletNode->setPosition( mObject->getOwnerNode()->getPosition());
    bulletNode->attachObject(bullet);

    Ogre::Vector3 fireDir = mObject->getFacing();
    MovingObject *bulletObject = new MovingObject(0.0f, fireDir * 5.0f + mObject->getVelocity() * 1.0f);
    bulletObject->setPosition(bulletNode->getPosition());
    bulletObject->setOwnerNode(bulletNode);
    bulletObject->setWorldBoundaries(1500.0f,0.0f,0.0f,1500.0f);
    bulletObject->setMaxSpeed(50.0f);

    objectsMap.insert(std::pair<unsigned int, MovingObject *>(++currentObjectIndex, bulletObject));
}

} // namespace Ap
