#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <CEGUI.h>
#include <CEGUI/RendererModules/OpenGLGUIRenderer/openglrenderer.h>
#include <CEGUIDefaultResourceProvider.h>

 /* ***********************************************************************
     Translate a SDLKey to the proper CEGUI::Key
 ************************************************************************ */
 CEGUI::uint SDLKeyToCEGUI(Uint8 key)
 {
     using namespace CEGUI;
     switch (key)
     {
     case SDLK_BACKSPACE:    return Key::Backspace;
     case SDLK_TAB:          return Key::Tab;
     case SDLK_RETURN:       return Key::Return;
     case SDLK_PAUSE:        return Key::Pause;
     case SDLK_ESCAPE:       return Key::Escape;
     case SDLK_SPACE:        return Key::Space;
     case SDLK_COMMA:        return Key::Comma;
     case SDLK_MINUS:        return Key::Minus;
     case SDLK_PERIOD:       return Key::Period;
     case SDLK_SLASH:        return Key::Slash;
     case SDLK_0:            return Key::Zero;
     case SDLK_1:            return Key::One;
     case SDLK_2:            return Key::Two;
     case SDLK_3:            return Key::Three;
     case SDLK_4:            return Key::Four;
     case SDLK_5:            return Key::Five;
     case SDLK_6:            return Key::Six;
     case SDLK_7:            return Key::Seven;
     case SDLK_8:            return Key::Eight;
     case SDLK_9:            return Key::Nine;
     case SDLK_COLON:        return Key::Colon;
     case SDLK_SEMICOLON:    return Key::Semicolon;
     case SDLK_EQUALS:       return Key::Equals;
     case SDLK_LEFTBRACKET:  return Key::LeftBracket;
     case SDLK_BACKSLASH:    return Key::Backslash;
     case SDLK_RIGHTBRACKET: return Key::RightBracket;
     case SDLK_a:            return Key::A;
     case SDLK_b:            return Key::B;
     case SDLK_c:            return Key::C;
     case SDLK_d:            return Key::D;
     case SDLK_e:            return Key::E;
     case SDLK_f:            return Key::F;
     case SDLK_g:            return Key::G;
     case SDLK_h:            return Key::H;
     case SDLK_i:            return Key::I;
     case SDLK_j:            return Key::J;
     case SDLK_k:            return Key::K;
     case SDLK_l:            return Key::L;
     case SDLK_m:            return Key::M;
     case SDLK_n:            return Key::N;
     case SDLK_o:            return Key::O;
     case SDLK_p:            return Key::P;
     case SDLK_q:            return Key::Q;
     case SDLK_r:            return Key::R;
     case SDLK_s:            return Key::S;
     case SDLK_t:            return Key::T;
     case SDLK_u:            return Key::U;
     case SDLK_v:            return Key::V;
     case SDLK_w:            return Key::W;
     case SDLK_x:            return Key::X;
     case SDLK_y:            return Key::Y;
     case SDLK_z:            return Key::Z;
     case SDLK_DELETE:       return Key::Delete;
     case SDLK_KP0:          return Key::Numpad0;
     case SDLK_KP1:          return Key::Numpad1;
     case SDLK_KP2:          return Key::Numpad2;
     case SDLK_KP3:          return Key::Numpad3;
     case SDLK_KP4:          return Key::Numpad4;
     case SDLK_KP5:          return Key::Numpad5;
     case SDLK_KP6:          return Key::Numpad6;
     case SDLK_KP7:          return Key::Numpad7;
     case SDLK_KP8:          return Key::Numpad8;
     case SDLK_KP9:          return Key::Numpad9;
     case SDLK_KP_PERIOD:    return Key::Decimal;
     case SDLK_KP_DIVIDE:    return Key::Divide;
     case SDLK_KP_MULTIPLY:  return Key::Multiply;
     case SDLK_KP_MINUS:     return Key::Subtract;
     case SDLK_KP_PLUS:      return Key::Add;
     case SDLK_KP_ENTER:     return Key::NumpadEnter;
     case SDLK_KP_EQUALS:    return Key::NumpadEquals;
     case SDLK_UP:           return Key::ArrowUp;
     case SDLK_DOWN:         return Key::ArrowDown;
     case SDLK_RIGHT:        return Key::ArrowRight;
     case SDLK_LEFT:         return Key::ArrowLeft;
     case SDLK_INSERT:       return Key::Insert;
     case SDLK_HOME:         return Key::Home;
     case SDLK_END:          return Key::End;
     case SDLK_PAGEUP:       return Key::PageUp;
     case SDLK_PAGEDOWN:     return Key::PageDown;
     case SDLK_F1:           return Key::F1;
     case SDLK_F2:           return Key::F2;
     case SDLK_F3:           return Key::F3;
     case SDLK_F4:           return Key::F4;
     case SDLK_F5:           return Key::F5;
     case SDLK_F6:           return Key::F6;
     case SDLK_F7:           return Key::F7;
     case SDLK_F8:           return Key::F8;
     case SDLK_F9:           return Key::F9;
     case SDLK_F10:          return Key::F10;
     case SDLK_F11:          return Key::F11;
     case SDLK_F12:          return Key::F12;
     case SDLK_F13:          return Key::F13;
     case SDLK_F14:          return Key::F14;
     case SDLK_F15:          return Key::F15;
     case SDLK_NUMLOCK:      return Key::NumLock;
     case SDLK_SCROLLOCK:    return Key::ScrollLock;
     case SDLK_RSHIFT:       return Key::RightShift;
     case SDLK_LSHIFT:       return Key::LeftShift;
     case SDLK_RCTRL:        return Key::RightControl;
     case SDLK_LCTRL:        return Key::LeftControl;
     case SDLK_RALT:         return Key::RightAlt;
     case SDLK_LALT:         return Key::LeftAlt;
     case SDLK_LSUPER:       return Key::LeftWindows;
     case SDLK_RSUPER:       return Key::RightWindows;
     case SDLK_SYSREQ:       return Key::SysRq;
     case SDLK_MENU:         return Key::AppMenu;
     case SDLK_POWER:        return Key::Power;
     default:                return 0;
     }
     return 0;
 }

void inject_mouse_down_to_cegui(Uint8 button)
{
  switch ( button )
  {
    case SDL_BUTTON_LEFT:
        CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
        break;
    case SDL_BUTTON_MIDDLE:
        CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
        break;
    case SDL_BUTTON_RIGHT:
        CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
        break;

    case SDL_BUTTON_WHEELDOWN:
        CEGUI::System::getSingleton().injectMouseWheelChange( -1 );
        break;
    case SDL_BUTTON_WHEELUP:
        CEGUI::System::getSingleton().injectMouseWheelChange( +1 );
        break;
  }
}
void inject_mouse_up_to_cegui(Uint8 button)
{
  switch ( button )
  {
    case SDL_BUTTON_LEFT:
        CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
        break;
    case SDL_BUTTON_MIDDLE:
        CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MiddleButton);
        break;
    case SDL_BUTTON_RIGHT:
        CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
        break;
  }
}

void inject_time_pulse_to_cegui()
{
    static Uint32 lastticks = SDL_GetTicks();   // This will be initialized only once!
    Uint32 ticks = SDL_GetTicks();
    CEGUI::System::getSingleton().injectTimePulse(0.001*float(ticks - lastticks));
    lastticks = ticks;
}

void inject_input_to_cegui(bool& must_quit)
{
    using namespace CEGUI;
    SDL_Event e;

    while (SDL_PollEvent(&e))         // go through all available events
    {
      switch (e.type)
      {
        case SDL_MOUSEMOTION:                            // we inject the mouse position directly.
            CEGUI::System::getSingleton().injectMousePosition(static_cast<float>(e.motion.x),static_cast<float>(e.motion.y));
            break;

        case SDL_MOUSEBUTTONDOWN:
            inject_mouse_down_to_cegui(e.button.button);     // let a special function handle the mouse button down event
            break;

        case SDL_MOUSEBUTTONUP:                   // let a special function handle the mouse button up event
            inject_mouse_up_to_cegui(e.button.button);
            break;

        case SDL_KEYDOWN:                         // to tell CEGUI that a key was pressed, we inject the scancode.
            CEGUI::System::getSingleton().injectKeyDown(SDLKeyToCEGUI(e.key.keysym.scancode));
            // as for the character it's a litte more complicated. we'll use for translated unicode value.
            if (e.key.keysym.unicode != 0) CEGUI::System::getSingleton().injectChar(e.key.keysym.unicode);
            break;

        case SDL_KEYUP:                           // like before we inject the scancode directly.
            CEGUI::System::getSingleton().injectKeyUp(e.key.keysym.scancode);
            break;

        case SDL_QUIT:
            must_quit = true;
            break;
        }
    }
    inject_time_pulse_to_cegui();   // Maybe needs to be done?!? -JL
}

void setupCEGUIResources() {
	using namespace CEGUI;
	DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>
    		(System::getSingleton().getResourceProvider());

	rp->setResourceGroupDirectory("schemes", "../../Media/gui/");
	rp->setResourceGroupDirectory("imagesets", "../../Media/gui/");
	rp->setResourceGroupDirectory("fonts", "../../Media/fonts/");
	rp->setResourceGroupDirectory("layouts", "../../Media/gui/");
	rp->setResourceGroupDirectory("looknfeels", "../../Media/gui/");

	// This is only needed if you are using Xerces and need to
	// specify the schemas location
	rp->setResourceGroupDirectory("schemas", "../../Media/gui/");

	// set the default resource groups to be used
	Imageset::setDefaultResourceGroup("imagesets");
	Font::setDefaultResourceGroup("fonts");
	Scheme::setDefaultResourceGroup("schemes");
	WidgetLookManager::setDefaultResourceGroup("looknfeels");
	WindowManager::setDefaultResourceGroup("layouts");

	SchemeManager *schemeManager = SchemeManager::getSingletonPtr();	
	Scheme *scheme = schemeManager->loadScheme("TaharezLookSkin.scheme", "schemes");
	assert( scheme );
	
	// Again, you only need to this one if you are using xerces and have
	// defined a group for schemas.
	//CEGUI::XercesParser::setSchemaDefaultResourceGroup("schemas");
}

void createGUIWindow()
{
    CEGUI::System *ceguiSystem= CEGUI::System::getSingletonPtr();
    assert(ceguiSystem);
    CEGUI::WindowManager *winMgr = CEGUI::WindowManager::getSingletonPtr();
    assert(winMgr);

    CEGUI::Window *ceguiRoot = winMgr->createWindow("DefaultGUISheet","root");
    assert(ceguiRoot);
    ceguiSystem->setGUISheet(ceguiRoot);

    CEGUI::UVector2 buttonSize = CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0));

    setupCEGUIResources();

    // Menu main page

    CEGUI::Window *mStateOverlay = winMgr->createWindow(
            (CEGUI::utf8*) "TaharezLook/Button",
            (CEGUI::utf8*) "root/playState/Button");

    mStateOverlay->setAlpha(0.5f);
    mStateOverlay->setText("Hello World!");
    mStateOverlay->setSize(buttonSize);
    mStateOverlay->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.6, 0)));

    ceguiRoot->addChildWindow(mStateOverlay);
}


const int WIDTH = 800;
const int HEIGHT = 600;
const int DEPTH = 24;

int main ( int argc, char** argv )
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE | SDL_OPENGLBLIT);
                                           //SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return 1;
    }

    // I suppose this is the essential OpenGL setup here:

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_FillRect(screen, NULL, SDL_MapRGBA(screen->format, 0, 0, 0, 0));

    glViewport(0,0, WIDTH,HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, float(WIDTH)/float(HEIGHT), 1.0, 100.0); // was 0.1, 100.0 -JL
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f,0.0f,0.0f,0.5f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    //glEnable(GL_CULL_FACE);
    //glDisable(GL_FOG);


    // This is the essential CEGUI setup!

    CEGUI::OpenGLRenderer *guirenderer = new CEGUI::OpenGLRenderer(0, WIDTH, HEIGHT);
    new CEGUI::System(guirenderer);
    SDL_EnableUNICODE(1);       // Supposedly makes stuff easier with CEGUI

    createGUIWindow();

    // load an image
    SDL_Surface* bmp = SDL_LoadBMP("cb.bmp");
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    // centre the bitmap on screen
    SDL_Rect dstrect;
    dstrect.x = (screen->w - bmp->w) / 2;
    dstrect.y = (screen->h - bmp->h) / 2;

    // program main loop
    bool done = false;
    while (!done)
    {
        inject_input_to_cegui(done);

        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
//                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
//                        done = true;
                    break;
                }
            } // end switch
        } // end of message processing

        // DRAWING STARTS HERE

        // Draw3D Stuff:

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGBA(screen->format, 0, 200, 0, 0));

        // draw bitmap
        SDL_BlitSurface(bmp, 0, screen, &dstrect);

        // draw CEGUI stuff
        CEGUI::System::getSingleton().renderGUI();

        SDL_UpdateRect(screen, 0, 0, 0, 0);
        SDL_GL_SwapBuffers();

        // DRAWING ENDS HERE

        // finally, update the screen :)
//        SDL_Flip(screen);
    } // end main loop

    // free loaded bitmap
    SDL_FreeSurface(bmp);

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
