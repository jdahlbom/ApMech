#include "PlayState.h"

#include <string>
#include <list>

#include <Ogre.h>

#include "net/netdata.h"
#include "Gui.h"
#include "Mech.h"
#include "Projectile.h"
#include "ScoreListing.h"
#include "types.h"

namespace ap {

PlayState::PlayState( GameStateManager *gameStateManager,
                      Ogre::SceneManager *sceneManager,
		      ap::Gui *gui,
		      const std::string &_ipAddress,
		      const std::string &_playerName) :
                      pSceneManager(sceneManager),
                      mCameraNodeParent(0),
		      pGui(gui),
                      mAvatarId(idForNoAvatar),
                      mObject(0),
		      ipAddress(_ipAddress),
		      playerName(_playerName)
{
    initStateManager(gameStateManager);

    pRoot = Ogre::Root::getSingletonPtr();

    Ogre::SceneNode *rootNode = pSceneManager->getRootSceneNode();
    mWorldCenter = rootNode->createChildSceneNode("Node/WorldCenter");

    mWorldCenter->setPosition(Ogre::Vector3(750,0,750));

    createGUIWindow();
}

PlayState::~PlayState() {}

void PlayState::enter( void ) {
    // Create the terrain

    #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    pSceneManager->setWorldGeometry(macBundlePath() + "/Contents/Resources/Media/terrain.cfg");
    #else
    pSceneManager->setWorldGeometry("terrain.cfg");
    #endif

    setupCamera(pSceneManager);

    // Create lighting
    createLighting(pSceneManager);

    netdata = new net::NetData(net::NetData::CLIENT);
    netdata->connect(ipAddress, 50740);
    netdata->me.nick = playerName;
    netdata->me.changed = true;         // Mark this info for transmission

    std::cout << "Entering PlayState" << std::endl;
}

void PlayState::exit( void ) {
    pSceneManager->destroyAllCameras(); // See Ogre API for warnings..
    pSceneManager->destroyAllLights();

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

    // NOTE: Aaaargh, this is not optimal! I didn't think of selecting objects to iterate through
    // based on the object's inheritance! Could it be done? Maybe. This is OK until then.
    while (ap::MovingObject *pMO = netdata->eachObject<ap::MovingObject *>(ap::OBJECT_TYPE_MECH)) pMO->updateNode();
    while (ap::MovingObject *pMO = netdata->eachObject<ap::MovingObject *>(ap::OBJECT_TYPE_PROJECTILE)) pMO->updateNode();


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
            case net::NetData::EVENT_MESSAGE:
                pGui->addChatItem(event.message->data);
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

    // TODO: Jukka, check this. Should destroy all entities in a scenenode and finally the node
    // itself. Every ap::MovingObject has its own scenenode, always, and these nodes have
    // nothing else, right? RIGHT?
    ap::MovingObject *pMO = dynamic_cast<ap::MovingObject *>(netdata->getObject(objectId));
    if (NULL != pMO) if (pMO->hasOwnerNode())
    {
        while (pMO->getOwnerNode()->numAttachedObjects() > 0)
        {
            Ogre::Entity *pEntity = dynamic_cast<Ogre::Entity *>(pMO->getOwnerNode()->getAttachedObject(0));
            assert(pEntity);
            pSceneManager->destroyEntity(pEntity);
            std::cout << "Destroyed an entity!"<<std::endl;
        }
        pSceneManager->destroySceneNode(pMO->getOwnerNode());
    }

    // Actually delete the object.
    netdata->removeObject(objectId);
}

void PlayState::setupCamera(Ogre::SceneManager *sceneManager) {
    mCamera = sceneManager->createCamera( "PlayerCamera" );

    // REQUIRES for the RenderTarget named "ApMech" to exist..
    Ogre::RenderTarget *renderWindow = pRoot->getRenderTarget("ApMech");
    assert(renderWindow);

    Ogre::ColourValue bgColor(0.2f, 0.6f, 0.2f);
    renderWindow->addViewport(mCamera)->setBackgroundColour(bgColor);

    // Attach a camera to the player model
    mCameraNode = sceneManager->createSceneNode("Node/MyCamera");
    mCameraNode->setPosition(Ogre::Vector3(0, 1000, -50));
    mCameraNode->pitch(Ogre::Degree(-85)); // Camera needs to look down.
    mCameraNode->attachObject(mCamera);
    mCamera->setNearClipDistance(5);

    assert(mWorldCenter);
    attachCameraNode(mWorldCenter);
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
  pGui->setupChatBox();
  pGui->setupScoreWindow();
  pGui->setChatReceiver(this);

  ScoreListing listing;
  ScoreTuple tuple;
  tuple.uid = 1;
  tuple.kills=11;
  tuple.deaths=111;
  tuple.score=1111;
  listing.addScore(tuple);
  tuple.uid = 2;
  tuple.kills = 22;
  tuple.deaths = 222;
  tuple.score = 2222;
  listing.addScore(tuple);

  std::cout << "PlayState::createGUIWindow():" << std::endl;
  listing.print();

  pGui->updateScores(listing);

  listing.clearAllScores();
  std::cout << "PS:cGUIW: cleared:" << std::endl;
  listing.print();

  tuple.kills=33;
  listing.addScore(tuple);
  tuple.uid=5;
  tuple.kills=55;
  tuple.deaths=555;
  tuple.score=5555;
  listing.addScore(tuple);
  listing.print();

  std::cout << "PS:cGUIW: added new: " << std::endl;
  //  listing.addScore(tuple);
  pGui->updateScores(listing);
}

bool PlayState::keyPressed( const ap::ooinput::KeyEvent &e ) {
  if (e.key == ooinput::AP_K_TAB) {
    pGui->showScoreWindow();
    return true;
  }

  if (pGui->keyPressed(e)) {
    return 1;
  }
    switch( e.key ) {
        case ooinput::AP_K_ESCAPE:
            std::cout << "Escape pressed, quitting." << std::endl;
            netdata->disconnect();
            this->requestShutdown();
            return 1;
        case ooinput::AP_K_a:
            if(mObject) {
                mObject->addClockwiseTurningSpeed(5);
                setNetDataDirty();
                return 1;
            }
            return 0;
        case ooinput::AP_K_w:
            if(mObject) {
                mObject->addForwardAcceleration(15);
                setNetDataDirty();
                return 1;
            }
            return 0;
        case ooinput::AP_K_s:
            if(mObject) {
                mObject->addForwardAcceleration(-9);
                setNetDataDirty();
                return 1;
            }
            return 0;
        case ooinput::AP_K_d:
            if(mObject) {
                mObject->addClockwiseTurningSpeed(-5);
                setNetDataDirty();
                return 1;
            }
            return 0;
        case ooinput::AP_K_SPACE:
            mObject->setFiring(true);
            setNetDataDirty();
            return 1;
        case ooinput::AP_K_t:
            pGui->activateChatBox();
            return true;
        default:
            std::cout << e.unicode << " pressed, not doing anything." << std::endl;
            return 0;
    }
  return 0;
}

bool PlayState::keyReleased( const ap::ooinput::KeyEvent &e ) {
  if (e.key == ooinput::AP_K_TAB) {
    pGui->hideScoreWindow();
    return true;
  }

  if(pGui->keyReleased(e)) {
    return 1;
  }

    switch( e.key ) {
        case ooinput::AP_K_a:
            if(mObject) {
                mObject->addClockwiseTurningSpeed(-5);
                setNetDataDirty();
		return 1;
            }
	    return 0;
        case ooinput::AP_K_w:
            if(mObject) {
                mObject->addForwardAcceleration(-15);
                setNetDataDirty();
		return 1;
            }
	    return 0;
        case ooinput::AP_K_s:
            if(mObject) {
                mObject->addForwardAcceleration(9);
                setNetDataDirty();
		return 1;
            }
	    return 0;
        case ooinput::AP_K_d:
            if(mObject) {
                mObject->addClockwiseTurningSpeed(5);
                setNetDataDirty();
		return 1;
            }
	    return 0;
        case ooinput::AP_K_SPACE:
            mObject->setFiring(false);
            setNetDataDirty();
	    return 1;
        default:
	  return 0;
    }
    return 0;
}

  bool PlayState::mousePressed(const ap::ooinput::MouseClickedEvent &e)
  {
    pGui->activateChatBox(false);
    return pGui->mousePressed(e);
  }

  bool PlayState::mouseReleased(const ap::ooinput::MouseClickedEvent &e)
  {
    return pGui->mouseReleased(e);
  }

  bool PlayState::mouseMoved(const ap::ooinput::MouseMovedEvent &e)
  {
    return pGui->mouseMoved(e);
  }

  void PlayState::sendChatMessage(const std::string &message)
  {
    NetMessage nmessage(message, netdata->me.uid);
    netdata->sendMessage(nmessage);
  }

void PlayState::createLighting(Ogre::SceneManager *sceneManager)
{
    sceneManager->setAmbientLight(Ogre::ColourValue(1,1,1));

    Ogre::Light *myLight= sceneManager->createLight("MyLight");
    myLight->setType(Ogre::Light::LT_DIRECTIONAL);
    myLight->setDiffuseColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    myLight->setSpecularColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    myLight->setDirection(Ogre::Vector3(0,-1,1));

    Ogre::Light *myLight2= sceneManager->createLight("MyLight2");
    myLight2->setType(Ogre::Light::LT_DIRECTIONAL);
    myLight2->setDiffuseColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    myLight2->setSpecularColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    myLight2->setDirection(Ogre::Vector3(1,0,-1));
}

} // namespace ap
