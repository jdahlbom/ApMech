#include <OgreRoot.h>
#include <OgreException.h>

#include <OgreRenderSystem.h>
#include <OgreCEGUIRenderer.h>
#include <CEGUI.h>

#include <boost/shared_ptr.hpp>

#include "GameStateManager.h"
#include "ooinput/InputSystem.h"
#include "ooinput/SDLInputSystem.h"
#include "functions.h"

SDL_Surface* setupSDL(int width, int height);
void setupOgre(int width, int height, Ogre::RenderWindow *&rWin, Ogre::SceneManager *&sceneMgr, Ogre::Root *&root);
void setupOgreResources();
void setupCEGUI(Ogre::RenderWindow *rWin, Ogre::SceneManager *sceneMgr);


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ) {
#else
int main( int argc, char **argv ) {
#endif

  //TODO: Should come from config file or other input.
    int width = 1060;
    int height = 600;

    SDL_Surface *screen = setupSDL(width, height);

    Ogre::SceneManager *sceneMgr = 0;
    Ogre::RenderWindow *renderWindow = 0;
    Ogre::Root *root = 0;
    setupOgre(width, height, renderWindow, sceneMgr, root);

    assert(renderWindow != 0);
    assert(sceneMgr != 0);
    setupCEGUI(renderWindow, sceneMgr);

    ap::ooinput::InputSystem *inputSystem = new ap::ooinput::SDLInputSystem();

    ap::GameStateManager * gameManager = new ap::GameStateManager(root, inputSystem, sceneMgr);

    try {
        // Initialise the game and switch to the first state
        gameManager->startGame();
    }
    catch ( Ogre::Exception& ex ) {
        #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
                MessageBox( NULL, ex.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL );
        #else
                std::cerr << "An exception has occured: " << ex.getFullDescription();
        #endif
    }

    delete gameManager;
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

  #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE

    Ogre::String resourcePath;
    resourcePath = ap::macBundlePath() + "/Contents/Resources/";
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
#ifdef WINDOWS
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWMInfo(&wmInfo);

    size_t winHandle = reinterpret_cast<size_t>(wmInfo.window);
    size_t winGlContext = reinterpret_cast<size_t>(wmInfo.hglrc);

    misc["externalWindowHandle"] = Ogre::StringConverter::toString(winHandle);
    misc["externalGLContext"] = Ogre::StringConverter::toString(winGlContext);
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
    mResourcePath = ap::macBundlePath() + "/Contents/Resources/";
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
                    Ogre::String(ap::macBundlePath() + "/" + archName), typeName, secName);
#else
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
#endif
        }
    }

    Ogre::LogManager::getSingleton().logMessage( "Resource directories setup" );
} // setupOgreResources


void setupCEGUI(Ogre::RenderWindow* renderWindow, Ogre::SceneManager *sceneMgr)
{
  using namespace CEGUI;

  /*
   * Setup CEGUI System and default GUI sheet.
   * States will access the System via getSystemPtr-method
   */

  OgreCEGUIRenderer *renderer = new OgreCEGUIRenderer(renderWindow,
						      Ogre::RENDER_QUEUE_OVERLAY,
						      false, 0, 
						      sceneMgr);

  System *mSystem = new System( renderer );
  SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");

  mSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
  mSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");

  WindowManager *mWin = WindowManager::getSingletonPtr();
  Window *ceguiRoot = mWin->createWindow("DefaultGUISheet", "root");
  
  mSystem->setGUISheet(ceguiRoot);    
  // Leaves renderer-pointer dangling. How to handle properly?

} // setupCEGUI
