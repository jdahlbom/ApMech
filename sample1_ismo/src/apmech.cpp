#include "Ogre.h"
#include "OIS/OIS.h"
#include "mech.h"


using namespace Ogre;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>

bool keepRendering = true;
Mech mech;


class MyFrameListener : public FrameListener, public OIS::KeyListener
{

	protected:
	
 	OIS::Keyboard *mKeyboard;
    OIS::InputManager* mInputManager;
	
	public:
	
	MyFrameListener(RenderWindow *window)
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
	
	virtual ~MyFrameListener() {}
	
	bool keyPressed(const OIS::KeyEvent &arg) {
	
		std::cout << "Key pressed!\n";
	    // give 'quitting' priority
		if (arg.key == OIS::KC_ESCAPE) {
			keepRendering = false;
			return false;
		}
		
		int x = mech.getX();
		int y = mech.getY();
		
		switch (arg.key) {
			case OIS::KC_UP:
				mech.setLocation(x, y+1);
				break;
			case OIS::KC_DOWN:
				mech.setLocation(x, y-1);
				break;
			case OIS::KC_LEFT:
				std::cout << "left key\n";
				mech.setLocation(x-1,y);
				break;
			case OIS::KC_RIGHT:
				std::cout << "right key\n";
				mech.setLocation(x+1, y);
				break;
			default:
				std::cout << "unknown key\n";
		}
		
		return true;
	}
	
	bool keyReleased(const OIS::KeyEvent &arg) {
		return true;
	}
	
	bool frameStarted(const FrameEvent &evt) { 
		// std::cout << "frame!\n";
		mKeyboard->capture();
		return true; 
	} 
};

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

#if 1

void loadResources(void)
{
	// Initialise, parse scripts etc
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void setupResources(void)
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

#endif


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{

	Root *root;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        Ogre::String resourcePath;
        resourcePath = macBundlePath() + "/Contents/Resources/";
        root = new Ogre::Root(resourcePath + "plugins.cfg",
                         resourcePath + "ogre.cfg", resourcePath + "Ogre.log");
#else
    root = new Root();
#endif

	// OpenGL
	RenderSystem *rSys = root->getRenderSystemByName("OpenGL Rendering Subsystem");
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
	 
 #if 1
 	root->initialise(true, "test window"); 
	RenderWindow *window = root->getAutoCreatedWindow(); 
 #else
  	root->initialise(false); 
	RenderWindow *window = root->createRenderWindow( 
		"Manual Ogre Window",  // window name 
		800,                   // window width, in pixels 
		600,                   // window height, in pixels 
		false,                 // fullscreen or not 
		0);    
#endif

	TextureManager::getSingleton().setDefaultNumMipmaps(5);
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	SceneManager *sceneMgr = root->createSceneManager(ST_EXTERIOR_CLOSE);
	sceneMgr->setWorldGeometry(resourcePath + "Media/terrain.cfg");


	sceneMgr->setAmbientLight( ColourValue( 1.0, 1.0, 0.9 ) );
	sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

	std::vector<Entity*> entities;
	std::vector<Entity*>::iterator entityIterator;


	mech.setLocation(0, 0);

	SceneNode *terrainCenterNode = sceneMgr->getRootSceneNode()->createChildSceneNode("terrainCenter", Vector3(750, 0, 750));
	
	Entity *robotEntity = sceneMgr->createEntity( "Robot", "robot.mesh" );
	SceneNode *robotNode = terrainCenterNode->createChildSceneNode("RobotNode");
	robotNode->attachObject( robotEntity );
	entities.push_back(robotEntity);

	for(entityIterator = entities.begin(); entityIterator != entities.end(); ++entityIterator) {
        (*entityIterator)->setCastShadows(true);
    }

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
	
	// add the event listener
	
	MyFrameListener *frameListener = new MyFrameListener(window);
	root->addFrameListener(frameListener);
	
	int i = 0;
	while (keepRendering) {
		i++;
		// std::cout << "Render frame " << i++ << "\n";

		robotNode->setPosition(Vector3(mech.getX(), 0, mech.getY()));
		root->renderOneFrame(); 
	} 

    return 0;
}
