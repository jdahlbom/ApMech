#include <vector>
#include <CEGUI/CEGUI.h>

#include "ExampleApplication.h"
#include "SampleFrameListener.h"
#include "OgreCEGUIRenderer.h"

class TutorialApplication : public ExampleApplication
{
protected:
    int terrainMaxX, terrainMaxY;
    CEGUI::System *mSystem;
    CEGUI::OgreCEGUIRenderer *mRenderer;

public:
    TutorialApplication():
        mSystem(0), mRenderer(0)
    {
        terrainMaxX = 1500;
        terrainMaxY = 1500;
    }

    ~TutorialApplication()
    {
        if(mSystem)
            delete(mSystem);
        if(mRenderer)
            delete(mRenderer);
    }
protected:
    void chooseSceneManager(void)
    {
        mSceneMgr=mRoot->createSceneManager(ST_EXTERIOR_CLOSE);
        mSceneMgr->setWorldGeometry("terrain.cfg");
    }

    void createScene(void)
    {
        this->mRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
        this->mSystem = new CEGUI::System(mRenderer);

        CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
        mSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
        mSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
        // Setting the default mouse cursor will result in always showing the cursor while
        // over a CEGUI window.
        CEGUI::MouseCursor::getSingleton().setImage(CEGUI::System::getSingleton().getDefaultMouseCursor());

        // Quit button
        CEGUI::WindowManager *win = CEGUI::WindowManager::getSingletonPtr();
        CEGUI::Window *sheet = win->createWindow("DefaultGUISheet", "CEGUIDemo/Sheet");

        CEGUI::Window *quit = win->createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        quit->setText("Quit");
        quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

        sheet->addChildWindow(quit);
        mSystem->setGUISheet(sheet);

        // Lighting

        mSceneMgr->setAmbientLight( ColourValue( 0.3, 0.3, 0.3 ) );
        mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

        std::vector<Entity*> entities;
        std::vector<Entity*>::iterator entityIterator;

        SceneNode *terrainCenterNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("terrainCenter", Vector3((int)terrainMaxX/2, 0,(int)terrainMaxY/2));

        Entity *robotEntity = mSceneMgr->createEntity( "Robot", "robot.mesh" );
        SceneNode *locationNode = terrainCenterNode->createChildSceneNode("SelfLocationNode");
        SceneNode *robotNode = locationNode->createChildSceneNode("SelfRotationNode");

        float robotDirectionYaw = 0.5f;
        robotNode->yaw(Radian(robotDirectionYaw));
        robotNode->attachObject( robotEntity );
        entities.push_back(robotEntity);


        Vector3 robotLocation = Vector3();
        Vector3 cameraOffset = Vector3(-200,1000,0);

        SceneNode *cameraNode = robotNode->createChildSceneNode("CameraNode", cameraOffset);
        cameraNode->attachObject(mCamera);

        for(entityIterator = entities.begin(); entityIterator != entities.end(); ++entityIterator)
        {
            (*entityIterator)->setCastShadows(true);
        }

        Light *light;
        light = mSceneMgr->createLight("Light1");
        light->setType(Light::LT_DIRECTIONAL);
        light->setDiffuseColour(ColourValue(1, 0, 0));
        light->setSpecularColour(ColourValue(1, 0, 0));
        light->setDirection(Vector3(0, -1, 1) );


       // CEGUI setup
        CEGUI::Texture *cTex = mRenderer->createTexture((CEGUI::utf8*)"R2TTex");

        CEGUI::Imageset *imageSet = CEGUI::ImagesetManager::getSingleton().createImageset((CEGUI::utf8*)"RttImageset", cTex);
        imageSet->defineImage((CEGUI::utf8*)"RttImage",
            CEGUI::Point(0.0f, 0.0f),
            CEGUI::Size(cTex->getWidth(), cTex->getHeight()),
            CEGUI::Point(0.0f,0.0f));

        CEGUI::Window *si = win->createWindow((CEGUI::utf8*)"TaharezLook/StaticImage", "RTTWindow");
        si->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.4f, 0)));
        si->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0, 0)));
        si->setProperty("Image", CEGUI::PropertyHelper::imageToString(&imageSet->getImage((CEGUI::utf8*)"RttImage")));

        sheet->addChildWindow(si);

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
        mCamera->setNearClipDistance(5);

        // Camera lies right above the player object, so pitching:
        float PI = 3.1415f;
        Radian quarter(0.5f * PI);
        Radian almostQuarter(0.4f * PI);
        mCamera->pitch(-almostQuarter);
        mCamera->yaw(-quarter);
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
