#include <Ogre.h>

#include "PlayState.h"



namespace Ap {

PlayState::PlayState( GameStateManager *gameStateManager,
                      Ogre::SceneManager *sceneManager) :
                      pSceneManager(sceneManager)
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
    mRobotNode->attachObject(myRobot);
    mRobotNode->setPosition(terrainCenter);

    // Attach a camera to the player model
    Ogre::SceneNode *cameraNode = rootNode->createChildSceneNode("Node/MyCamera");
    cameraNode->setPosition(mRobotNode->getPosition() + Ogre::Vector3(0, 250,0));
    cameraNode->lookAt(mRobotNode->getPosition() + Ogre::Vector3(0,0,-1), Ogre::Node::TS_WORLD);
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
}

void PlayState::pause( void ) {
}

void PlayState::resume( void ) {
}

void PlayState::update( unsigned long lTimeElapsed ) {

}

//-----------------------------------------------------------------------------

void PlayState::createGUIWindow()
{
    CEGUI::System *ceguiSystem= CEGUI::System::getSingletonPtr();
    CEGUI::Window *ceguiRoot = ceguiSystem->getGUISheet();
    CEGUI::WindowManager *winMgr = CEGUI::WindowManager::getSingletonPtr();
    CEGUI::UVector2 buttonSize = CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0));

    // Menu main page

    mStateOverlay = winMgr->createWindow("TaharezLook/Button", "root/playState/Button");
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
        case AP_K_SPACE:
            std::cout << "Space pressed, quitting." << std::endl;
            this->requestShutdown();
            break;
        default:
            std::cout << e.unicode << " pressed, not doing anything." << std::endl;
            break;
    }
  return false;
}

bool PlayState::keyReleased( const Ap::KeyEvent &e ) {
  return false;
}

} // namespace Ap
