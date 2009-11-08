#include "PlayState.h"

#include <string>
#include <list>

#ifndef WIN32
#include <Ogre.h>
#else
#include <Ogre/Ogre.h>
#endif

#include "net/netdata.h"
#include "Gui.h"
#include "Mech.h"
#include "ObjectDataModel.h"
#include "Projectile.h"
#include "ScoreListing.h"
#include "GameWorld.h"
#include "functions.h"
#include "types.h"

namespace ap {

    PlayState::PlayState( GameStateManager *gameStateManager,
            Ogre::SceneManager *sceneManager,
            ap::Gui *gui,
            net::NetData *netdata,
            const std::string &_playerName) :
        pSceneManager(sceneManager),
        mCameraNodeParent(0),
        pGui(gui),
        mAvatarId(idForNoAvatar),
        mObject(0),
        netdata(netdata),
        playerName(_playerName)
    {
    initStateManager(gameStateManager);

    pRoot = Ogre::Root::getSingletonPtr();

    Ogre::SceneNode *rootNode = pSceneManager->getRootSceneNode();
    mWorldCenter = rootNode->createChildSceneNode("Node/WorldCenter");

    mWorldCenter->setPosition(Ogre::Vector3(750,0,750));

    createGUIWindow();

    pDataModel = ObjectDataModel::getInstance();
}

PlayState::~PlayState() {}

void PlayState::enter( void ) {
    assert(netdata);    // If no netdata, we're doomed! We must be connected too, but that's not checked. FIXME maybe!
    GameWorld *gWorld;
    do {
        mSleep(2);   // sleep 2 milliseconds
        netdata->receiveChanges();
        gWorld = dynamic_cast<GameWorld *>(netdata->getFirstObjectOfType(OBJECT_TYPE_GAMEWORLD));
    } while (gWorld == NULL);   // WAIT until we get a world

    // Create the terrain
    #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    pSceneManager->setWorldGeometry(macBundlePath() + "/Contents/Resources/" + gWorld->mapFileName);
    #else
    pSceneManager->setWorldGeometry(gWorld->mapFileName);   // for example "data/maps/example.map"
    #endif

    setupCamera(pSceneManager);

    // Create lighting
    createLighting(pSceneManager);

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
    dt = float(lTimeElapsed)*0.001; // dt is in seconds

//    while (ap::MovingObject *pMO = netdata->eachObject<ap::MovingObject *>(ap::OBJECT_TYPE_MECH)) pMO->advance(dt);
    while (ap::MovingObject *pMO = netdata->eachObject<ap::MovingObject *>(ap::OBJECT_TYPE_PROJECTILE)) pMO->advance(dt);

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
                setAvatar(event.id);
                break;
            case net::NetData::EVENT_DELETEOBJECT:
                deleteNetObject(event.id);
                break;
            case net::NetData::EVENT_CREATEOBJECT:
            {
                NetObject *pObject = netdata->getObject(event.uid);
                if (pObject) {
                    switch (pObject->getObjectType()) {
                        case ap::OBJECT_TYPE_SCORELISTING:
                            pScoreListing = dynamic_cast<ap::ScoreListing *>(pObject);
                            updateScores();
                            break;
                        case ap::OBJECT_TYPE_MECH:
                        case ap::OBJECT_TYPE_PROJECTILE:
                            createSceneNodeForMovable(event.uid);
                            break;
                        default:
                            break;
                    }
                }
                break;
	        }
            case net::NetData::EVENT_UPDATEOBJECT:
            {
                switch (event.objectType) {
                    case ap::OBJECT_TYPE_SCORELISTING:
                        updateScores(); // at the moment updateScores is the only update event.
                        break;
                    case ap::OBJECT_TYPE_MECH:
                    {
                        // A mech has probably just been painted! Well, set its color again anyway.
                        ap::Mech *pMech = netdata->getObject<ap::Mech*>(event.id);
                        float r, g, b;
                        r = float(pMech->color & 0x0000FF) / 255.0f;
                        g = float(pMech->color & 0x00FF00) / 65535.0f;
                        b = float(pMech->color & 0xFF0000) / 16777215.0f;
                        if (pMech->getEntity())
                            pMech->getEntity()->getSubEntity(0)->setCustomParameter(1, Ogre::Vector4(r, g, b, 0.0f));
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
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

    // Phew! This is *some* trouble to get the Entity for our avatar object.
    // TODO: Maybe there is (or should be made) a simpler way to get the Entity..
//    dynamic_cast<Ogre::Entity*>(pAvatarObject->getOwnerNode()->getAttachedObject(0))->getSubEntity(0)->setCustomParameter(1, Ogre::Vector4(1.f, 0.f, 0.8f, 0.0f));
}

void PlayState::createSceneNodeForMovable(uint32 objectId)
{
    // FIXME: Nasty having to cast things...
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
    Ogre::Entity *newEntity = NULL;
    assert(newObject->hasOwnerNode());

    std::stringstream ss;
    ss << "Entity/"<<objectId;

    Ogre::SceneNode *objNode = newObject->getOwnerNode();

    switch (newObject->getObjectType() ) {
    case ap::OBJECT_TYPE_MECH:
      {
        float r, g, b;

        ap::Mech *pMech = dynamic_cast<ap::Mech *>(newObject);
        r = float(pMech->color & 0x0000FF) / 255.0f;
        g = float(pMech->color & 0x00FF00) / 65535.0f;
        b = float(pMech->color & 0xFF0000) / 16777215.0f;

        std::string mesh = pDataModel->getMeshFilename(ap::OBJECT_TYPE_MECH);
        newEntity = pSceneManager->createEntity(ss.str(), mesh);
        newEntity->getSubEntity(0)->setCustomParameter(1, Ogre::Vector4(r, g, b, 0.0f));
        pMech->setEntity(newEntity);
      }
      break;
    case ap::OBJECT_TYPE_PROJECTILE:
      {
        std::string mesh = pDataModel->getMeshFilename(ap::OBJECT_TYPE_PROJECTILE);
        newEntity = pSceneManager->createEntity(ss.str(), mesh);

        ap::Projectile *pProj = dynamic_cast<ap::Projectile *>(newObject);
        pProj->setEntity(newEntity);
      }
      break;
    default:
      // TODO: Should cause an error?
      return;
    }

    if (newEntity)
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
                mObject->setClockwiseTurningSpeed(1.0f);
                setNetDataDirty();
                return 1;
            }
            return 0;
        case ooinput::AP_K_w:
            if(mObject) {
                mObject->setForwardAcceleration(1.0f);
                setNetDataDirty();
                return 1;
            }
            return 0;
        case ooinput::AP_K_s:
            if(mObject) {
                mObject->setForwardAcceleration(-1.0f);
                setNetDataDirty();
                return 1;
            }
            return 0;
        case ooinput::AP_K_d:
            if(mObject) {
                mObject->setClockwiseTurningSpeed(-1.0f);
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
        return true;
    }

    switch( e.key ) {
        case ooinput::AP_K_a:
            if(mObject) {
                mObject->setClockwiseTurningSpeed(0);
                setNetDataDirty();
                return true;
            }
            return false;
        case ooinput::AP_K_w:
            if(mObject) {
                mObject->setForwardAcceleration(0);
                setNetDataDirty();
                return true;
            }
            return false;
        case ooinput::AP_K_s:
            if(mObject) {
                mObject->setForwardAcceleration(0);
                setNetDataDirty();
                return true;
            }
            return false;
        case ooinput::AP_K_d:
            if(mObject) {
                mObject->setClockwiseTurningSpeed(0);
                setNetDataDirty();
                return true;
            }
            return false;
        case ooinput::AP_K_SPACE:
            mObject->setFiring(false);
            setNetDataDirty();
            return true;
        default:
            return false;
    }
    return false;
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

 void PlayState::updateScores() const {
   if (pGui && pScoreListing) {
     pGui->updateScores(*pScoreListing);
   }
 }

} // namespace ap
