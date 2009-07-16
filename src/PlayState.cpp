#include "PlayState.h"

#include <Ogre.h>
#include "net/netdata.h"

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

namespace ap {

PlayState::PlayState( GameStateManager *gameStateManager,
                      Ogre::SceneManager *sceneManager) :
                      pSceneManager(sceneManager),
                      mCameraNodeParent(0),
                      mAvatarId(idForNoAvatar),
                      currentObjectIndex(0)
{
    initStateManager(gameStateManager);

    pRoot = Ogre::Root::getSingletonPtr();
    createGUIWindow();

// TODO: Refaktoroi, kopioitu nablasta.
    /*netdata = new NetData(NetData::CLIENT);
    netdata->connect("127.0.0.1", 5074);
    netdata->me.nick = "Test";
    netdata->me.changed = true;         // Mark this info for transmission */
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

    mSelfNode = rootNode->createChildSceneNode("Node/MyRobot");
    mSelfNode->setPosition(terrainCenter);
    mSelfNode->attachObject(myRobot);

    mObject = new MovingObject();
    mObject->setPosition(terrainCenter);
    mObject->setOwnerNode(mSelfNode);
    mObject->setWorldBoundaries(1500.0f,0.0f,0.0f,1500.0f);
    mObject->setMaxSpeed(25.0f);
    objectsMap.insert(std::pair<unsigned int, MovingObject *>(++currentObjectIndex, mObject));

    mWorldCenter = rootNode->createChildSceneNode("Node/WorldCenter");
    mWorldCenter->setPosition(terrainCenter);

    // Attach a camera to the player model
    mCameraNode = pSceneManager->createSceneNode("Node/MyCamera");
    mCameraNode->setPosition(Ogre::Vector3(-100, 500,0));
    mCameraNode->yaw(Ogre::Degree(-90));  // Camera facing: Z+ --> X+
    mCameraNode->pitch(Ogre::Degree(-75)); // Camera needs to look down.
    //cameraNode->lookAt(mSelfNode->getPosition(), Ogre::Node::TS_WORLD);
    mCameraNode->attachObject(mCamera);
    mCamera->setNearClipDistance(5);
    attachCameraNode(mWorldCenter);

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


    netdata = new net::NetData(net::NetData::CLIENT);
    netdata->connect("127.0.0.1", 50740);
    netdata->me.nick = "Test";
    netdata->me.changed = true;         // Mark this info for transmission

    std::cout << "Entering PlayState" << std::endl;
}

void PlayState::exit( void ) {
    mStateOverlay->deactivate();
    mStateOverlay->hide();
    std::cout << "Exiting PlayState" << std::endl;

    delete(netdata);

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
  //  TODO: Should update the scenenodes, should not update the state data.
    for( std::map<unsigned int, MovingObject*>::iterator it = objectsMap.begin(); it!=objectsMap.end(); ++it) {
        it->second->update(lTimeElapsed);
    }

    net::NetEvent *event=0;
    while (netdata->pollEvent(event)) {
        switch( event->type ) {
            case net::NetData::EVENT_SETAVATAR:
                setAvatar(event->uid);
                break;
            case net::NetData::EVENT_DELETEOBJECT:
                deleteNetObject(event->uid);
                break;
            case net::NetData::EVENT_CREATEOBJECT:
                createSceneNodeForMovable(event->uid);
                break;
            default:
                break;
        }
    }
}

void PlayState::setAvatar(int avatarId)
{
    if ( avatarId == mAvatarId )
        return;

    // FIXME: Nasty having to cast things...
    ap::MovingObject *pAvatarObject = dynamic_cast<ap::MovingObject *>(netdata->getNetObject(avatarId));
    if (!pAvatarObject->hasOwnerNode()) {
        createSceneNodeForMovable(avatarId);
    }
    Ogre::SceneNode *pAvatarNode = pAvatarObject->getOwnerNode();
    attachCameraNode(pAvatarNode);
}

void PlayState::createSceneNodeForMovable(int objectId)
{
    // FIXME: Nasty having to cast things...
    ap::MovingObject *pAvatarObject = dynamic_cast<ap::MovingObject *>(netdata->getNetObject(objectId));
    if (!pAvatarObject->hasOwnerNode()) {
        std::stringstream ss;
        ss << "Node/NetObject/"<<objectId;
        Ogre::SceneNode *rootNode = pSceneManager->getRootSceneNode();
        Ogre::SceneNode *objectNode = rootNode->createChildSceneNode(ss.str());
        pAvatarObject->setOwnerNode(objectNode);
    }
}

void PlayState::deleteNetObject(int objectId)
{
    // Detach camera from the object to be deleted.
    if ( objectId == mAvatarId ) {
        attachCameraNode(mWorldCenter);
    }
    // Actually delete the object.
    netdata->removeNetObject(objectId);
}

void PlayState::attachCameraNode(Ogre::SceneNode *newParentNode)
{
    if (0 != mCameraNodeParent) {
        mCameraNodeParent->removeChild(mCameraNode);
    }
    newParentNode->addChild(mCameraNode);
    mCameraNodeParent = newParentNode;
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
    mStateOverlay->setText("ESC to quit, WASD to move");
    mStateOverlay->setSize(buttonSize);
    mStateOverlay->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.6, 0)));

    mStateOverlay->deactivate();
    mStateOverlay->hide();
    ceguiRoot->addChildWindow(mStateOverlay);

    std::cout << "PlayState::createGUIWindow finished" << std::endl;
}

bool PlayState::keyPressed( const ap::ooinput::KeyEvent &e ) {
    switch( e.key ) {
        case ooinput::AP_K_ESCAPE:
            std::cout << "Escape pressed, quitting." << std::endl;
            this->requestShutdown();
            break;
        case ooinput::AP_K_a:
            mObject->addClockwiseTurningSpeed(5);
            break;
        case ooinput::AP_K_w:
            mObject->addForwardAcceleration(15);
            break;
        case ooinput::AP_K_s:
            mObject->addForwardAcceleration(-9);
            break;
        case ooinput::AP_K_d:
            mObject->addClockwiseTurningSpeed(-5);
            break;
        case ooinput::AP_K_SPACE:
            fireGun();
            break;
        default:
            std::cout << e.unicode << " pressed, not doing anything." << std::endl;
            break;
    }
  return false;
}

bool PlayState::keyReleased( const ap::ooinput::KeyEvent &e ) {
    switch( e.key ) {
        case ooinput::AP_K_a:
            mObject->addClockwiseTurningSpeed(-5);
            break;
        case ooinput::AP_K_w:
            mObject->addForwardAcceleration(-15);
            break;
        case ooinput::AP_K_s:
            mObject->addForwardAcceleration(9);
            break;
        case ooinput::AP_K_d:
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

} // namespace ap
