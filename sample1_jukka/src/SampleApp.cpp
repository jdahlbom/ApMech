#include <vector>

#include "ExampleApplication.h"
#include "SampleFrameListener.h"

class TutorialApplication : public ExampleApplication
{
protected:
    int terrainMaxX, terrainMaxY;

public:
    TutorialApplication()
    {
        terrainMaxX = 1500;
        terrainMaxY = 1500;
    }

    ~TutorialApplication()
    {
    }
protected:
    void chooseSceneManager(void)
    {
        mSceneMgr=mRoot->createSceneManager(ST_EXTERIOR_CLOSE);
        mSceneMgr->setWorldGeometry("terrain.cfg");
    }

    void createScene(void)
    {

        mSceneMgr->setAmbientLight( ColourValue( 1.0, 1.0, 0.9 ) );
        mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

        std::vector<Entity*> entities;
        std::vector<Entity*>::iterator entityIterator;

        SceneNode *terrainCenterNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("terrainCenter", Vector3(750, 0, 750));

        Entity *robotEntity = mSceneMgr->createEntity( "Robot", "robot.mesh" );
        SceneNode *robotNode = terrainCenterNode->createChildSceneNode("RobotNode");
        robotNode->attachObject( robotEntity );
        entities.push_back(robotEntity);

        for(entityIterator = entities.begin(); entityIterator != entities.end(); ++entityIterator)
        {
            (*entityIterator)->setCastShadows(true);
        }

        Light *light;
        light = mSceneMgr->createLight("Light3");
        light->setType(Light::LT_DIRECTIONAL);
        light->setDiffuseColour(ColourValue(1, 0, 0));
        light->setSpecularColour(ColourValue(1, 0, 0));
        light->setDirection(Vector3(0, -1, 1) );
    }

    virtual void createFrameListener(void)
    {
        mFrameListener = new TutorialFrameListener(mWindow, mCamera, mSceneMgr);
        mRoot->addFrameListener(mFrameListener);
        // Show the frame stats overlay
        mFrameListener->showDebugOverlay(true);

    }

  virtual void createCamera(void)
  {
      mCamera = mSceneMgr->createCamera("PlayerCam");
      Vector3 robotLocation = Vector3((int)terrainMaxX/2, 0,(int)terrainMaxY/2);
      //mCamera->setPosition(robotLocation + Vector3(0,100,0));
      mCamera->setPosition(Vector3(850, 300, 750));
      mCamera->lookAt(Vector3(750, 20, 750));
      //mCamera->lookAt(robotLocation);
      mCamera->setNearClipDistance(5);
  }

  virtual void createViewports(void)
  {
    Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
  }

};

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
    TutorialApplication app;

    try {
        app.go();
    } catch( Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n",
                e.what());
#endif
    }

    return 0;
}
