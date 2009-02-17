#include "apgui.h"
#include "graphics.h"
#include "gameengine.h"
#include "gameobject.h"
#include "apframelistener.h"
#include "apeventhandler.h"
#include "Ogre.h"

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



Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

bool Graphics::initialize(GameEngine *engine)
{
    mEngine = engine;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    Ogre::String resourcePath;
    resourcePath = macBundlePath() + "/Contents/Resources/";
    mRoot = new Ogre::Root(resourcePath + "plugins.cfg",
                          resourcePath + "ogre.cfg", resourcePath + "Ogre.log");
#else
    mRoot = new Root();

#endif

	RenderSystemList* rlist = mRoot->getAvailableRenderers();

    // OpenGL
    mRenderSystem = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	
	if(mRenderSystem == NULL)
		throw ("null render system!");

    mRenderSystem->setConfigOption("Full Screen", "No");
    mRoot->setRenderSystem(mRenderSystem);

    setupResources();
    // loadResources();

    // end gracelessly if the preferred renderer is not available
    if (mRoot->getRenderSystem() == NULL) {
        std::cout << "ERROR: render system is NULL\n";
        delete mRoot;
        return 1;
    }

    mRoot->restoreConfig();

    mRoot->showConfigDialog();

    mRoot->initialise(true, "Achtung Panzer! (test window)");
    mWindow = mRoot->getAutoCreatedWindow();

    loadResources();

    mSceneMgr= mRoot->createSceneManager(ST_EXTERIOR_CLOSE);

    // add the GUI
    mGuiOverlay = new ApGui(mWindow, mSceneMgr, mEngine->getEventHandler());
    // add the event listener
    // Should pass the eventhandler to framelistener as well..?
    ApFrameListener *frameListener = new ApFrameListener(mWindow, mEngine->getEventHandler());
    mRoot->addFrameListener(frameListener);
    return true;
}

bool Graphics::loadTerrain()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    mSceneMgr->setWorldGeometry(macBundlePath() + "/Contents/Resources/Media/terrain.cfg");
#else
    mSceneMgr->setWorldGeometry("terrain.cfg");
#endif
    return true;
}


bool Graphics::loadResources(void)
{
    // Initialise, parse scripts etc
    TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    return true;
}

void Graphics::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    Ogre::String mResourcePath;
    mResourcePath = macBundlePath() + "/Contents/Resources/";
    cf.load(mResourcePath + "resources.cfg");
#else
    cf.load("resources.cfg");
#endif

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // OS X does not set the working directory relative to the app,
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    Ogre::String(macBundlePath() + "/" + archName), typeName, secName);
#else
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
#endif
        }
    }

    Ogre::LogManager::getSingleton().logMessage( "Resource directories setup" );

}

bool Graphics::updateGraphics(GameObject *gameObject)
{
    SceneNode *node;

    std::cout << "updateGraphics" << std::endl;
    std::cout << "Object location: " << gameObject->getLocation() << std::endl;

    /* get the graphical representation from the object */
    if (!gameObject->isVisible())
        return false;

    node = gameObject->getSceneNode();

    if (node == NULL) {

        // draw this object
        SceneNode *corner = mSceneMgr->getRootSceneNode()->createChildSceneNode("terrainCorner", Vector3::ZERO);
        Entity *robotEntity = mSceneMgr->createEntity(gameObject->id_s, "robot.mesh");

        node = corner->createChildSceneNode(gameObject->id_s);
        node->attachObject(robotEntity);
        // entities.push_back(robotEntity);
        gameObject->setSceneNode(node);
    }

	if (mCamera != NULL && node != NULL) {

		Vector3 mechloc = node->getPosition();
		mCamera->setPosition(mechloc + (Vector3(-150, 125, 25)));
		mCamera->setAutoTracking(true, node, Vector3(300, 0, 0));

		//gameObject->getSceneNode()->attachObject(mCamera);

		//Vector3 mechloc = node->getPosition();
		//mCamera->setPosition(mechloc + (Vector3(-150, 125, 25)));
		//mCamera->setAutoTracking(true, node, Vector3(300, 0, 0));
	}

    node->setPosition(gameObject->getLocation());

    return true;
}

bool Graphics::load()
{
    loadTerrain();

    mSceneMgr->setAmbientLight( ColourValue( 1.0, 1.0, 0.9 ) );
    mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

    Light *light;
    light = mSceneMgr->createLight("Light3");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDiffuseColour(ColourValue(1, 0, 0));
    light->setSpecularColour(ColourValue(1, 0, 0));
    light->setDirection(Vector3(0, -1, 1) );

    mCamera = mSceneMgr->createCamera("PlayerCam");
    Vector3 camGroundOrigin = Vector3(750, 0, 750);
    mCamera->setPosition(camGroundOrigin + Vector3(0, 250, 0));
    mCamera->lookAt(camGroundOrigin + Vector3(1, 0, 0));
    mCamera->setNearClipDistance(5);

    Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

    return true;
}

bool Graphics::render()
{

    if (mRoot) {
        mRoot->renderOneFrame();
        return true;
    }
    return false;
}
