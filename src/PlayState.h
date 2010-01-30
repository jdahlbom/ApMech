#ifndef AP_PLAYSTATE_H
#define AP_PLAYSTATE_H

#include <list>
#include <string>

#ifndef WIN32
#include <OgreCamera.h>
#else
#include <Ogre/OgreCamera.h>
#endif

#include "GameState.h"

#include "Gui.h"
#include "GuiChatReceiver.h"
#include "GuiMouseReceiver.h"
#include "ObjectDataModel.h"
#include "MovingObject.h"
#include "ScoreListing.h"
#include "net/netdata.h"
#include "types.h"
#include "constants.h"


namespace ap {
  class ActionKeyMap;
  class GameStateManager;

  class PlayState : public GameState, public GuiChatReceiver, public GuiMouseReceiver {
public:
    PlayState( GameStateManager *gameStateManager,
	       Ogre::SceneManager *pSceneManager,
	       ap::Gui *gui,
	       ap::net::NetData *netdata,
	       ap::ActionKeyMap *akMap);
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

    // GuiMouseReceiver
    void receiveMousePosition(int x, int y);

private:
    Ogre::Root                  *pRoot;
    Ogre::Camera                *mCamera;
    Ogre::Viewport              *mViewport;
    Ogre::SceneManager          *pSceneManager;

    Ogre::SceneNode             *mSelfNode;
    Ogre::SceneNode             *mWorldCenter;
    Ogre::SceneNode             *mCameraNode;
    Ogre::SceneNode             *mCameraNodeParent;

    ap::ScoreListing            *pScoreListing;

    ap::Gui                     *pGui;
    ap::net::NetData            *netdata;
    ap::ActionKeyMap            *pActionKMap;

    std::string                 ipAddress;
    ObjectDataModel             *pDataModel;

    static const int            idForNoAvatar=-1;
    uint32                      mAvatarId;
    ap::uint32                  unresolvedAvatarId;

    MovingObject                *mObject;

    std::list<Ogre::ParticleSystem *> particleSystemList;

    float                       dt;                 // delta-t between two updates in seconds. Used in update( )

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
    void updateScores() const;

};

} // namespace ap
#endif
