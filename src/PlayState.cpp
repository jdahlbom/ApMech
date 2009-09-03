#include "PlayState.h"

#include <Ogre.h>

#include "net/netdata.h"
#include "Mech.h"
#include "Projectile.h"
#include "types.h"

namespace ap {

PlayState::PlayState( GameStateManager *gameStateManager,
                      Ogre::SceneManager *sceneManager) :
                      pSceneManager(sceneManager),
                      mCameraNodeParent(0),
                      mAvatarId(idForNoAvatar),
                      mObject(0)
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
    Ogre::SceneNode *rootNode = pSceneManager->getRootSceneNode();

    mWorldCenter = rootNode->createChildSceneNode("Node/WorldCenter");
    mWorldCenter->setPosition(terrainCenter);

    // Attach a camera to the player model
    mCameraNode = pSceneManager->createSceneNode("Node/MyCamera");
    mCameraNode->setPosition(Ogre::Vector3(-100, 500,0));
    //mCameraNode->yaw(Ogre::Degree(-90));  // Camera facing: Z- --> X+
    mCameraNode->pitch(Ogre::Degree(-75)); // Camera needs to look down.
    mCameraNode->attachObject(mCamera);
    mCamera->setNearClipDistance(5);
    attachCameraNode(mWorldCenter);

    // Create lighting
    createLighting(pSceneManager);

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
}

void PlayState::pause( void ) {
}

void PlayState::resume( void ) {
}

void PlayState::update( unsigned long lTimeElapsed ) {
    netdata->receiveChanges();

  //  TODO: Should update the scenenodes, should not update the state data.
/*JL
    ap::net::NetData::netObjectsType::iterator objectIter;
    for(objectIter=netdata->netobjects.begin(); objectIter!=netdata->netobjects.end(); ++objectIter)
    {
        ap::MovingObject *obj = dynamic_cast<ap::MovingObject *>(objectIter->second);
        if( !obj )
            continue;

        obj->updateNode();
    }
*/
    // Should be equal to above.
    // NOTE: Aaaargh, this is not optimal! I didn't think of selecting objects to iterate through
    // based on the object's inheritance! Could it be done? Maybe. This is OK until then.
    while (ap::MovingObject *mop = netdata->eachObject<ap::MovingObject *>(ap::OBJECT_TYPE_MECH)) mop->updateNode();
    while (ap::MovingObject *mop = netdata->eachObject<ap::MovingObject *>(ap::OBJECT_TYPE_PROJECTILE)) mop->updateNode();


    net::NetEvent event;
    while (netdata->pollEvent(event)) {
        switch( event.type ) {
            case net::NetData::EVENT_SETAVATAR:
                setAvatar(event.uid);
                break;
            case net::NetData::EVENT_DELETEOBJECT:
                deleteNetObject(event.uid);
                break;
            case net::NetData::EVENT_CREATEOBJECT:
                std::cout << "[PLAYSTATE] Handled EVENT_CREATEOBJECT, uid: " << event.uid << std::endl;
                createSceneNodeForMovable(event.uid);
                break;
            default:
                break;
        }
    }

    netdata->sendChanges();
}

void PlayState::setAvatar(uint32 avatarId)
{
    std::cout << "[PLAYSTATE] Set avatar to id " << avatarId << std::endl;
    assert(0!=netdata);
    if ( avatarId == mAvatarId )
        return;

    // FIXME: Nasty having to cast things...
    ap::MovingObject *pAvatarObject = dynamic_cast<ap::MovingObject *>(netdata->getObject(avatarId));
    if (!pAvatarObject->hasOwnerNode()) {
        createSceneNodeForMovable(avatarId);
    }
    Ogre::SceneNode *pAvatarNode = pAvatarObject->getOwnerNode();
    attachCameraNode(pAvatarNode);
    netdata->me.setControlPtr(pAvatarObject->getControlPtr());
    mObject = pAvatarObject;
}

void PlayState::createSceneNodeForMovable(uint32 objectId)
{
    // FIXME: Nasty having to cast things...
    std::cout << "[PLAYSTATE]<createSceneNodeForMovable> object id: " << objectId << " [eol] " << std::endl;
    ap::MovingObject *pAvatarObject = dynamic_cast<ap::MovingObject *>(netdata->getObject(objectId));
    if (!pAvatarObject->hasOwnerNode()) {
        std::stringstream ss;
        ss << "Node/NetObject/"<<objectId;
        Ogre::SceneNode *rootNode = pSceneManager->getRootSceneNode();
        Ogre::SceneNode *objectNode = rootNode->createChildSceneNode(ss.str());
        pAvatarObject->setOwnerNode(objectNode);

        createNewEntity(pAvatarObject, objectId);
    }
}

void PlayState::createNewEntity(ap::MovingObject *newObject, uint32 objectId)
{
    assert(newObject->hasOwnerNode());

    std::stringstream ss;
    ss << "Entity/"<<objectId;

    Ogre::SceneNode *objNode = newObject->getOwnerNode();

    std::stringstream mesh;
    if (dynamic_cast<ap::Mech *>(newObject)) {
        mesh << "CrudeMech.mesh";
    } else if (dynamic_cast<ap::Projectile *>(newObject)) {
        mesh << "CrudeMissile.mesh";
    }
    Ogre::Entity *newEntity = pSceneManager->createEntity(ss.str(), mesh.str());
    objNode->attachObject(newEntity);
}


void PlayState::deleteNetObject(uint32 objectId)
{
    // Detach camera from the object to be deleted.
    if ( objectId == mAvatarId ) {
        attachCameraNode(mWorldCenter);
        netdata->me.setControlPtr(0);
    }

    // Actually delete the object.
    netdata->removeObject(objectId);
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
            (CEGUI::utf8*) "root/playState/Button");

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
            netdata->disconnect();
            this->requestShutdown();
            break;
        case ooinput::AP_K_a:
            if(mObject) {
                mObject->addClockwiseTurningSpeed(5);
                setNetDataDirty();
            }
            break;
        case ooinput::AP_K_w:
            if(mObject) {
                mObject->addForwardAcceleration(15);
                setNetDataDirty();
            }
            break;
        case ooinput::AP_K_s:
            if(mObject) {
                mObject->addForwardAcceleration(-9);
                setNetDataDirty();
            }
            break;
        case ooinput::AP_K_d:
            if(mObject) {
                mObject->addClockwiseTurningSpeed(-5);
                setNetDataDirty();
            }
            break;
        case ooinput::AP_K_SPACE:
            mObject->setFiring(true);
            setNetDataDirty();
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
            if(mObject) {
                mObject->addClockwiseTurningSpeed(-5);
                setNetDataDirty();
            }
            break;
        case ooinput::AP_K_w:
            if(mObject) {
                mObject->addForwardAcceleration(-15);
                setNetDataDirty();
            }
            break;
        case ooinput::AP_K_s:
            if(mObject) {
                mObject->addForwardAcceleration(9);
                setNetDataDirty();
            }
            break;
        case ooinput::AP_K_d:
            if(mObject) {
                mObject->addClockwiseTurningSpeed(5);
                setNetDataDirty();
            }
            break;
        case ooinput::AP_K_SPACE:
            mObject->setFiring(false);
            setNetDataDirty();
        default:
            break;
    }

  return false;
}

void PlayState::createLighting(Ogre::SceneManager *sceneManager)
{
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
}

} // namespace ap
