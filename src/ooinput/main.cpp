#include <SDL.h>
#include <Ogre.h>

#include "TestListener.h"
#include "SDLInputSystem.h"

using namespace Ogre;

// Taken from:
// http://www.ogre3d.org/wiki/index.php/Using_SDL_Input

int main(void)
{

    SDL_Init(SDL_INIT_VIDEO);

//Then create the window. By passing in SDL_OPENGL, we are telling SDL
//to create an OpenGL context:

    SDL_Surface *screen = SDL_SetVideoMode(640, 480, 0, SDL_OPENGL);

//SDL is now setup. Now we setup OGRE. The first step is to create
//the OGRE Root object:

    Root *root = new Root("plugins.cfg", "ogre.cfg", "ogre.log");

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
                                String("Could not restore config!"),
                                String("ApOOInput main func"));
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

    misc["externalWindowHandle"] = StringConverter::toString(winHandle);
    misc["externalGLContext"] = StringConverter::toString(winGlContext);
#else
    misc["currentGLContext"] = String("True");
#endif

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
    RenderWindow *renderWindow = root->createRenderWindow("ApOOInputWindow",
                                                            640, 480,
                                                            false,
                                                            &misc);
//And that's all the setup done!

//To make OGRE render to your SDL window, you simply tell OGRE to
//render one frame and then tell SDL to swap the buffers or update
//the display (depending on whether you are using double buffering):

    Ap::SDLInputSystem *input = new Ap::SDLInputSystem();
    Ap::TestListener *listening = new Ap::TestListener();
    input->addKeyboardListener(listening);

    while(listening->running) {
        input->capture();

        root->renderOneFrame();
        SDL_GL_SwapBuffers();
    }

    delete(listening);
    delete(input);
}
