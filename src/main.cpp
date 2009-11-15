#ifndef WIN32
#include <OgreRoot.h>
#include <OgreException.h>
#include <OgreRenderSystem.h>
#include <OgreCEGUIRenderer.h>
#include <SDL.h>
#else
#include <CEGUIRenderer/OgreCEGUIRenderer.h>
#include <Ogre/OgreRoot.h>
#include <Ogre/OgreException.h>
#include <Ogre/OgreRenderSystem.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#endif

#include <CEGUI/CEGUI.h>

#include "definitions.h"
#include "GameStateManager.h"
#include "Gui.h"
#include "ooinput/InputSystem.h"
#include "ooinput/SDLInputSystem.h"
#include "functions.h"

SDL_Surface* setupSDL(int width, int height);
void setupOgre(int width, int height, Ogre::RenderWindow *&rWin, Ogre::SceneManager *&sceneMgr, Ogre::Root *&root);
void setupOgreResources();
CEGUI::Renderer *setupCEGUI(Ogre::RenderWindow *rWin, Ogre::SceneManager *sceneMgr);


#if AP_PLATFORM == AP_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ) {
#else
int main( int argc, char **argv ) {
#endif
    int width, height;  // 1060 x 600 ?

    // Load resolution from a config file (ogre.cfg for now - I guess it's ok)
    Ogre::ConfigFile ogrecfg;
    ogrecfg.load("ogre.cfg");
    Ogre::String renderSystem = ogrecfg.getSetting("Render System");
    Ogre::String videoMode = ogrecfg.getSetting("Video Mode", renderSystem);
    Ogre::StringVector videoModeVector = Ogre::StringUtil::split(videoMode);
    if (videoModeVector.size() == 3) {
        width = Ogre::StringConverter::parseInt(videoModeVector[0]);
        height = Ogre::StringConverter::parseInt(videoModeVector[2]);
    } else {
        width = 800; height = 600;
    }

    SDL_Surface *screen = setupSDL(width, height);

    Ogre::SceneManager *sceneMgr = 0;
    Ogre::RenderWindow *renderWindow = 0;
    Ogre::Root *root = 0;
    setupOgre(width, height, renderWindow, sceneMgr, root);

    assert(renderWindow != 0);
    assert(sceneMgr != 0);
    CEGUI::Renderer *ceguiRenderer = setupCEGUI(renderWindow, sceneMgr);
    ap::Gui *gui = new ap::Gui(ceguiRenderer);

    ap::ooinput::InputSystem *inputSystem = new ap::ooinput::SDLInputSystem();

    ap::GameStateManager * gameManager = new ap::GameStateManager(root, inputSystem, sceneMgr, gui);

    try {
        // Initialise the game and switch to the first state
        gameManager->startGame();
    }
    catch ( Ogre::Exception& ex ) {
		std::cerr << "An exception has occured: " << ex.getFullDescription();
    }

    delete gameManager;
    delete inputSystem;
    delete gui;
    delete ceguiRenderer;
    delete root;
    return 0;
} // main


SDL_Surface* setupSDL(int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL);

    assert(screen);
    return screen;
} // setupSDL


 void setupOgre(int width, int height, Ogre::RenderWindow *&rWin, Ogre::SceneManager *&sceneMgr, Ogre::Root *&root) {
  // TODO: use a ogre.cfg or similar with proper information.

  #if AP_PLATFORM == AP_PLATFORM_OSX

    Ogre::String resourcePath;
    resourcePath = ap::bundlePath() + "/Contents/Resources/";
    root = new Ogre::Root(resourcePath + "plugins.cfg",
                          resourcePath + "ogre.cfg", resourcePath + "Ogre.log");
  #else
    root =  new Ogre::Root("plugins.cfg", "ogre.cfg", "ogre.log");

  #endif
    assert(root);

    // TODO: Is plugins.cfg really used at the moment?

    if(!root->restoreConfig()) {
        std::cout << "Cannot restore config" << std::endl;
        throw new Ogre::Exception(0,
                                Ogre::String("Could not restore config!"),
                                Ogre::String("ApMech   main func"));
    }
    root->initialise(false);

    // Hook Ogre into SDL window.
    Ogre::NameValuePairList misc;
#ifdef WIN32_LEAN_AND_MEAN
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWMInfo(&wmInfo);

    size_t winHandle = reinterpret_cast<size_t>(wmInfo.window);
    size_t winGlContext = reinterpret_cast<size_t>(wmInfo.hglrc);

    misc["externalWindowHandle"] = Ogre::StringConverter::toString(winHandle);
    misc["externalGLContext"] = Ogre::StringConverter::toString(winGlContext);
	misc["externalGLControl"] = Ogre::String("True");
#else
    misc["currentGLContext"] = Ogre::String("True");
#endif

    setupOgreResources();

    Ogre::RenderWindow *renderWindow = 0;
    renderWindow = root->createRenderWindow("ApMech",
					    width,
					    height,
					    false,
					    &misc);
    assert(renderWindow != 0);

    renderWindow->setVisible(true);

    // passing out pointers. Ugly, but ach-well..
    rWin = renderWindow;
    // Create needed scenemanagers
    // ST_EXTERIOR_CLOSE is Ogre's simple Terrain scene manager. Not paging, otherwise good.
    sceneMgr = root->createSceneManager( Ogre::ST_EXTERIOR_CLOSE, "ST_EXTERIOR_CLOSE" );
    assert(sceneMgr != 0);

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
} // setupOgre

void setupOgreResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    Ogre::String mResourcePath;
    mResourcePath = ap::bundlePath() + "/Contents/Resources/";
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
#if AP_PLATFORM == AP_PLATFORM_OSX
            // OS X does not set the working directory relative to the app,
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    Ogre::String(ap::bundlePath() + "/" + archName), typeName, secName);
#else
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
#endif
        }
    }

    Ogre::LogManager::getSingleton().logMessage( "Resource directories setup" );
} // setupOgreResources


CEGUI::Renderer* setupCEGUI(Ogre::RenderWindow* renderWindow, Ogre::SceneManager *sceneMgr)
{
  CEGUI::OgreCEGUIRenderer *renderer =
    new CEGUI::OgreCEGUIRenderer(renderWindow,
				 Ogre::RENDER_QUEUE_OVERLAY,
				 false, 0,
				 sceneMgr);
  assert(renderer);
  return renderer;
} // setupCEGUI
