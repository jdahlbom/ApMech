#include "graphics.h"
#include "gameengine.h"
#include "gameobject.h"

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

MyFrameListener::MyFrameListener(RenderWindow *window, GameEngine *engine): engine(engine), window(window)
{
    // using buffered input
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    window->getCustomAttribute("test window", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("test window"), windowHndStr.str()));
    
    mInputManager = OIS::InputManager::createInputSystem( pl );
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    // mInputManager->addKeyListener(this, "game");
    mKeyboard->setEventCallback(this); 
}

bool MyFrameListener::keyPressed(const OIS::KeyEvent &arg)
{
    std::cout << "Key pressed!\n";
    
    return engine->processKbEvent(arg.key);
}

bool MyFrameListener::keyReleased(const OIS::KeyEvent &arg)
{
    return true;
}

bool MyFrameListener::frameStarted(const FrameEvent &evt)
{
    // std::cout << "frame!\n";
    mKeyboard->capture();
    return true; 
}


Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

bool Graphics::initialize(GameEngine *engine)
{
    this->engine = engine;
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	Ogre::String resourcePath;
    resourcePath = macBundlePath() + "/Contents/Resources/";
    root = new Ogre::Root(resourcePath + "plugins.cfg",
                          resourcePath + "ogre.cfg", resourcePath + "Ogre.log");
#else
    root = new Root();
#endif
    
	// OpenGL
	rSys = root->getRenderSystemByName("OpenGL Rendering Subsystem");
	rSys->setConfigOption("Full Screen", "No");
	root->setRenderSystem(rSys);
    
    setupResources();
	// loadResources();
    
	// end gracelessly if the preferred renderer is not available 
	if (root->getRenderSystem() == NULL) { 
		std::cout << "ERROR: render system is NULL\n";
		delete root; 
		return 1; 
	}
	
	root->restoreConfig();
	
	root->showConfigDialog();
    
 	root->initialise(true, "test window"); 
	window = root->getAutoCreatedWindow(); 
    
	loadResources();
	
	// add the event listener
	MyFrameListener *frameListener = new MyFrameListener(window, engine);
	root->addFrameListener(frameListener);
	return true;
}

bool Graphics::loadTerrain()
{
	sceneMgr = root->createSceneManager(ST_EXTERIOR_CLOSE);
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	sceneMgr->setWorldGeometry(macBundlePath() + "/Contents/Resources/Media/terrain.cfg");
#else
	sceneMgr->setWorldGeometry("terrain.cfg");
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

bool Graphics::updateGraphics(GameObject *o)
{
	SceneNode *node;
    
    std::cout << "updateGraphics" << std::endl;
    
	/* get the graphical representation from the object */
	if (!o->isVisible())
		return false;
    
	node = o->getGraphics();
	
	if (node == NULL) {
        
		// draw this object
		SceneNode *corner = sceneMgr->getRootSceneNode()->createChildSceneNode("terrainCorner", Vector3(0, 0, 0));
		Entity *robotEntity = sceneMgr->createEntity(o->id_s, "robot.mesh");
        
		node = corner->createChildSceneNode(o->id_s);
		node->attachObject(robotEntity);
		// entities.push_back(robotEntity);		
		o->setGraphics(node);
	}
	
	
	node->setPosition(Vector3(o->getX(), 0, o->getY()));
	
	return true;
}

bool Graphics::load()
{
	loadTerrain();
    
	sceneMgr->setAmbientLight( ColourValue( 1.0, 1.0, 0.9 ) );
	sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
    
	std::vector<Entity*> entities;
	std::vector<Entity*>::iterator entityIterator;
    
    
	// mech.setLocation(0, 0, 0);
#if 0

	SceneNode *terrainCenterNode = sceneMgr->getRootSceneNode()->createChildSceneNode("terrainCenter", Vector3(750, 0, 750));
	Entity *robotEntity = sceneMgr->createEntity("Robot", "robot.mesh");
	SceneNode *robotNode = terrainCenterNode->createChildSceneNode("RobotNode");
	robotNode->attachObject( robotEntity );
	entities.push_back(robotEntity);
    
	for(entityIterator = entities.begin(); entityIterator != entities.end(); ++entityIterator) {
        (*entityIterator)->setCastShadows(true);
    }
#endif
    
	Light *light;
	light = sceneMgr->createLight("Light3");
	light->setType(Light::LT_DIRECTIONAL);
	light->setDiffuseColour(ColourValue(1, 0, 0));
	light->setSpecularColour(ColourValue(1, 0, 0));
	light->setDirection(Vector3(0, -1, 1) );
	
	Camera *mCamera = sceneMgr->createCamera("PlayerCam");
    Vector3 robotLocation = Vector3((int)1500/2, 0,(int)1500/2);
    //mCamera->setPosition(robotLocation + Vector3(0,100,0));
    mCamera->setPosition(Vector3(850, 300, 750));
    mCamera->lookAt(Vector3(750, 20, 750));
    //mCamera->lookAt(robotLocation);
    mCamera->setNearClipDistance(5);
	
	Viewport* vp = window->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
	
	return true;
}

bool Graphics::render()
{
    if (root) {
        root->renderOneFrame();
        return true;
    }
    return false;
}
