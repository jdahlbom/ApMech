#ifndef AP_PLAYSTATE_H
#define AP_PLAYSTATE_H

#include <OgreCamera.h>
#include <CEGUI.h>

#include "GameState.h"
#include "MovingObject.hpp"
#include "net/netdata.h"
#include "types.h"

namespace ap {

class PlayState : public GameState {
public:
    PlayState( GameStateManager *gameStateManager,
                Ogre::SceneManager *pSceneManager);
    ~PlayState( void );

    void enter( void );
    void exit( void );

    void pause( void );
    void resume( void );
    void update( unsigned long lTimeElapsed );

    void createGUIWindow();

//--KeyboardListener-(inherited through GameState)-----------------------------
    bool keyPressed( const ooinput::KeyEvent &e );
    bool keyReleased( const ooinput::KeyEvent &e );

private:
    Ogre::Root                  *pRoot;
    Ogre::Camera                *mCamera;
    Ogre::Viewport              *mViewport;
    Ogre::SceneManager          *pSceneManager;
    CEGUI::Window               *mStateOverlay;

    Ogre::SceneNode             *mSelfNode;
    Ogre::SceneNode             *mWorldCenter;
    Ogre::SceneNode             *mCameraNode;
    Ogre::SceneNode             *mCameraNodeParent;

    ap::net::NetData            *netdata;

    static const int            idForNoAvatar=-1;
    int                         mAvatarId;

    MovingObject                *mObject;

    void fireGun();
    void setAvatar(uint32 avatarId);
    void deleteNetObject(uint32 objectId);
    void attachCameraNode(Ogre::SceneNode *newParentNode);
    void createSceneNodeForMovable(uint32 objectId);
    void createLighting(Ogre::SceneManager *sceneManager);
    void setNetDataDirty() { netdata->me.changed = true; }
    void createNewEntity(ap::MovingObject *newObject, uint32 objectId);

};

} // namespace ap
#endif
