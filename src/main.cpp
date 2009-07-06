#include <OgreRoot.h>
#include <OgreException.h>

#include <OgreRenderSystem.h>
#include <OgreCEGUIRenderer.h>
#include <CEGUI.h>

#include <boost/shared_ptr.hpp>

#include "GameStateManager.h"
#include "ooinput/InputSystem.h"
#include "ooinput/SDLInputSystem.h"

void setupResources(void);
std::string macBundlePath(void);


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ) {
#else
int main( int argc, char **argv ) {
#endif

// Taken from:
// http://www.ogre3d.org/wiki/index.php/Using_SDL_Input

    SDL_Init(SDL_INIT_VIDEO);

//Then create the window. By passing in SDL_OPENGL, we are telling SDL
//to create an OpenGL context:

    SDL_Surface *screen = SDL_SetVideoMode(1280, 720, 0, SDL_OPENGL);

//SDL is now setup. Now we setup OGRE. The first step is to create
//the OGRE Root object:

    Ogre::Root *root;

//In your OGRE configuration files, make sure you are using the OpenGL renderer.
//This is my ogre.cfg, which tells OGRE to use OpenGL for rendering, to disable
//full-screen mode (feel free to remove) and that we are rendering in a 640x480
//sized window:

/*
Render System=OpenGL Rendering Subsystem
[OpenGL Rendering Subsystem]
Full Screen=No
Video Mode=640 x 480
*/


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE

    Ogre::String resourcePath;
    resourcePath = macBundlePath() + "/Contents/Resources/";
    root = new Ogre::Root(resourcePath + "plugins.cfg",
                          resourcePath + "ogre.cfg", resourcePath + "Ogre.log");
#else
    root =  new Ogre::Root("plugins.cfg", "ogre.cfg", "ogre.log");

#endif

//The OGRE OpenGL renderer requires the appropriate plugins to be loaded. Here
//is my plugins.cfg. Note that I have removed plugins I do not use here. You
//might not want something so minimal in your own plugins.cfg:

/*
PluginFolder=ogre/lib
Plugin=RenderSystem_GL.so
Plugin=Plugin_OctreeSceneManager.so
*/

//With the Root object created, we proceed to initialise it.
//The 'false' we pass to initialise() tells OGREe to not auto-create
//a window (since we have already done this with SDL):

    if(!root->restoreConfig()) {
        std::cout << "Cannot restore config" << std::endl;
        throw new Ogre::Exception(0,
                                Ogre::String("Could not restore config!"),
                                Ogre::String("ApMech   main func"));
    }
    root->initialise(false);

//Nearly there! We now need to configure OGRE to blindly render to
//the current OpenGL context (which will be the one we have just created
//with SDL). To do this, we need to set some named parameters (OGRE's way
//of setting configuration properties).

//Windows and Linux require a slightly different set of named parameters.
//Ideally, it would be cool if this part was portable too. But like I said,
//this is experimental - hopefully this will be cleaned up in future
//releases of OGRE!

//Here's the code to setup the named parameters (named misc in the below
//code) for Linux and Windows (with the appropriate code under the
//appropriate #ifdef):

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

    setupResources();

//What we have done above is use the externalGLContext named parameter
//for Windows and the currentGLContext named parameter for Linux. They
//both do the same thing in the end - make OGRE issue OpenGL commands
//to the OpenGL context we have created with SDL.

//I will elaborate a bit more. currentGLContext makes OGRE blindly issue
//OpenGL commands - it is assumed the OpenGL context has already been
//setup. This is ideally what we want for all platforms, but it seems
//the Windows OGRE OpenGL renderer does not support this. The Windows
//OpenGL renderer has something similar though - externalGLContext and
//externalWindowHandle. These named parameters require you to pass them
//two Windows-specific handles - namely the Windows-specific handle to
//the window and the Windows-specific handle to the OpenGL context.
//Thankfully, SDL provides us with this information!

//Now that we have the named parameters setup, we pass it to the
//createRenderWindow() function, in the OGRE Root object. This will not
//actually create a render window. Instead, it will just "link" OGRE up
//with the window we created with SDL and make OGRE render to that:

//RenderWindow *renderWindow = root->createRenderWindow("MainRenderWindow", 640, 480, false, &misc);
    Ogre::RenderWindow *renderWindow = root->createRenderWindow("ApMech",
                                                            1280, 720,
                                                            false,
                                                            &misc);
    renderWindow->setVisible(true);


    // Create needed scenemanagers
    Ogre::SceneManager *mSceneMgr = root->createSceneManager( Ogre::ST_EXTERIOR_CLOSE, "ST_EXTERIOR_CLOSE" );

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        /*
     * Setup CEGUI System and default GUI sheet.
     * States will access the System via getSystemPtr-method
     */

    CEGUI::OgreCEGUIRenderer *mRenderer = new CEGUI::OgreCEGUIRenderer( renderWindow,
                                                                    Ogre::RENDER_QUEUE_OVERLAY,
                                                                    false, 0, mSceneMgr);

    CEGUI::System *mSystem = new CEGUI::System( mRenderer );
    CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");

    mSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
    mSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");

    CEGUI::WindowManager *mWin = CEGUI::WindowManager::getSingletonPtr();
    CEGUI::Window *ceguiRoot = mWin->createWindow("DefaultGUISheet", "root");

    mSystem->setGUISheet(ceguiRoot);

    /*
     * CEGUI initialization complete
     */

    ap::ooinput::InputSystem *inputSystem = new ap::ooinput::SDLInputSystem();

    ap::GameStateManager * gameManager = new ap::GameStateManager(root, renderWindow, inputSystem, mSceneMgr);
//    boost::shared_ptr<Ap::GameStateManager> gameManager(new Ap::GameStateManager(root, renderWindow, inputSystem, mSceneMgr));

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

    return 0;
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
