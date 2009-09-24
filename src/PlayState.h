#ifndef AP_PLAYSTATE_H
#define AP_PLAYSTATE_H

#include <list>
#include <string>

#include <OgreCamera.h>

#include "GameState.h"

#include "Gui.h"
#include "GuiChatReceiver.h"
#include "MovingObject.hpp"
#include "net/netdata.h"
#include "types.h"
#include "constants.h"


namespace ap {

  class PlayState : public GameState, public GuiChatReceiver{
public:
    PlayState( GameStateManager *gameStateManager,
	       Ogre::SceneManager *pSceneManager,
	       ap::Gui *gui,
	       const std::string &ipAddress,
	       const std::string &playerName);
    ~PlayState( void );

    void enter( void );
    void exit( void );

    void pause( void );
    void resume( void );
    void update( unsigned long lTimeElapsed );

//--KeyboardListener-(inherited through GameState)-----------------------------
    bool keyPressed( const ooinput::KeyEvent &e );
    bool keyReleased( const ooinput::KeyEvent &e );

    // MouseListener
    bool mousePressed(const ooinput::MouseClickedEvent &e);
    bool mouseReleased(const ooinput::MouseClickedEvent &e);
    bool mouseMoved(const ooinput::MouseMovedEvent &e);

    // GuiChatReceiver
    void sendChatMessage(const std::string &message);

private:
    Ogre::Root                  *pRoot;
    Ogre::Camera                *mCamera;
    Ogre::Viewport              *mViewport;
    Ogre::SceneManager          *pSceneManager;

    Ogre::SceneNode             *mSelfNode;
    Ogre::SceneNode             *mWorldCenter;
    Ogre::SceneNode             *mCameraNode;
    Ogre::SceneNode             *mCameraNodeParent;

    ap::Gui                     *pGui;
    ap::net::NetData            *netdata;

    std::string                 ipAddress;
    std::string                 playerName;

    static const int            idForNoAvatar=-1;
    int                         mAvatarId;

    MovingObject                *mObject;

    void fireGun();
    void setAvatar(uint32 avatarId);
    void deleteNetObject(uint32 objectId);

    void setupCamera(Ogre::SceneManager *sceneManager);
    void attachCameraNode(Ogre::SceneNode *newParentNode);
    void createSceneNodeForMovable(uint32 objectId);
    void createLighting(Ogre::SceneManager *sceneManager);
    void setNetDataDirty() { netdata->me.changed = true; }
    void createNewEntity(ap::MovingObject *newObject, uint32 objectId);
    void createGUIWindow();

};

} // namespace ap
#endif
