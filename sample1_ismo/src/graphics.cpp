#include "graphics.h"
#include "gameengine.h"
#include "gameobject.h"
#include "apframelistener.h"
#include "apeventhandler.h"
#include "apgui.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
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
    this->engine_ = engine;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    Ogre::String resourcePath;
    resourcePath = macBundlePath() + "/Contents/Resources/";
    root_ = new Ogre::Root(resourcePath + "plugins.cfg",
                          resourcePath + "ogre.cfg", resourcePath + "Ogre.log");
#else
    root_ = new Root();
#endif

    // OpenGL
    rSys_ = root_->getRenderSystemByName("OpenGL Rendering Subsystem");
    rSys_->setConfigOption("Full Screen", "No");
    root_->setRenderSystem(rSys_);

    setupResources();
    // loadResources();

    // end gracelessly if the preferred renderer is not available
    if (root_->getRenderSystem() == NULL) {
        std::cout << "ERROR: render system is NULL\n";
        delete root_;
        return 1;
    }

    root_->restoreConfig();

    root_->showConfigDialog();

    root_->initialise(true, "Achtung Panzer! (test window)");
    window_ = root_->getAutoCreatedWindow();

    loadResources();

    sceneMgr_= root_->createSceneManager(ST_EXTERIOR_CLOSE);

    // get event handler for the engine
    ApEventHandler *eventHandler = engine_->getEventHandler();
    // add the GUI
    ApGui *guiOverlay = new ApGui(window_, sceneMgr_, eventHandler);
    // add the event listener
    // guiSystem -parametri on ruma häcki koska JD ei keksinyt parempaa tapaa äkkiseltään.
    ApFrameListener *frameListener = new ApFrameListener(window_, engine_, guiOverlay->getSystemPtr());
    root_->addFrameListener(frameListener);
    return true;
}

bool Graphics::loadTerrain()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    sceneMgr_->setWorldGeometry(macBundlePath() + "/Contents/Resources/Media/terrain.cfg");
#else
    sceneMgr_->setWorldGeometry("terrain.cfg");
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

    /* get the graphical representation from the object */
    if (!gameObject->isVisible())
        return false;

    node = gameObject->getSceneNode();

    if (node == NULL) {

        // draw this object
        SceneNode *corner = sceneMgr_->getRootSceneNode()->createChildSceneNode("terrainCorner", Vector3(0, 0, 0));
        Entity *robotEntity = sceneMgr_->createEntity(gameObject->id_s, "robot.mesh");

        node = corner->createChildSceneNode(gameObject->id_s);
        node->attachObject(robotEntity);
        // entities.push_back(robotEntity);
        gameObject->setSceneNode(node);
    }


    node->setPosition(Vector3(gameObject->getX(), 0, gameObject->getY()));

    return true;
}

bool Graphics::load()
{
    loadTerrain();

    sceneMgr_->setAmbientLight( ColourValue( 1.0, 1.0, 0.9 ) );
    sceneMgr_->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

    std::vector<Entity*> entities;
    std::vector<Entity*>::iterator entityIterator;


    Light *light;
    light = sceneMgr_->createLight("Light3");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDiffuseColour(ColourValue(1, 0, 0));
    light->setSpecularColour(ColourValue(1, 0, 0));
    light->setDirection(Vector3(0, -1, 1) );

    Camera *mCamera = sceneMgr_->createCamera("PlayerCam");
    Vector3 robotLocation = Vector3((int)1500/2, 0,(int)1500/2);
    //mCamera->setPosition(robotLocation + Vector3(0,100,0));
    mCamera->setPosition(Vector3(850, 300, 750));
    mCamera->lookAt(Vector3(750, 20, 750));
    //mCamera->lookAt(robotLocation);
    mCamera->setNearClipDistance(5);

    Viewport* vp = window_->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

    return true;
}

bool Graphics::render()
{
    if (root_) {
        root_->renderOneFrame();
        return true;
    }
    return false;
}
