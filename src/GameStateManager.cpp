#include "GameStateManager.h"

#include <string>

#include <Ogre.h>
#include <OgreWindowEventUtilities.h>
#include <SDL.h>

#include "Gui.h"

#include "GameState.h"
#include "LoginState.h"
#include "PlayState.h"
#include "functions.h"

namespace ap {

GameStateManager::GameStateManager(
		    Ogre::Root 	 *root,
                    ooinput::InputSystem *inputSystem,
                    Ogre::SceneManager *sceneManager,
		    ap::Gui *gui) :
    mRoot(root),
    mSceneMgr(sceneManager),
    pGui(gui),
    pInputMgr( inputSystem ),
    mLoginState(0),
    mPlayState(0),
    bShutdown( false ) {}

GameStateManager::~GameStateManager( void )
{
    // Clean up all the states
    while( !mStates.empty() ) {
        mStates.back()->exit();
        mStates.pop_back();
    }

    delete mLoginState;
    mLoginState = 0;

    delete mPlayState;
    mPlayState  = 0;
}

void GameStateManager::startGame() {

    // Setup states
  mLoginState  = new LoginState( this, mSceneMgr, pGui);


    // Setup input
    pInputMgr->addKeyboardListener( this );
    pInputMgr->addMouseListener( this );

    // Change to first state
    this->changeState( mLoginState );

    // lTimeLastFrame remembers the last time that it was checked
    // We use it to calculate the time since last frame
    unsigned long lTimeLastFrame = 0;

    // Main while-loop
    while( !bShutdown ) {
        // Calculate time since last frame and remember current time for next frame
        unsigned long lTimeCurrentFrame = getTicks();
        unsigned long lTimeSinceLastFrame = lTimeCurrentFrame - lTimeLastFrame;
        lTimeLastFrame = lTimeCurrentFrame;

        // Update inputmanager
        pInputMgr->capture();

        // Update current state
        mStates.back()->update( lTimeSinceLastFrame );

        // Render next frame
        mRoot->renderOneFrame();

        SDL_GL_SwapBuffers();

        // Deal with platform specific issues
        Ogre::WindowEventUtilities::messagePump();

        if(lTimeSinceLastFrame < 10) {
            ap::mSleep(10.0f - lTimeSinceLastFrame);
        }
    }
}

void GameStateManager::changeState( GameState *gameState ) {
    // Cleanup the current state stack
    while( !mStates.empty() ) {
        mStates.back()->exit();
        mStates.pop_back();
    }

    // Store and init the new state
    mStates.push_back( gameState );
    mStates.back()->enter();
}

void GameStateManager::pushState( GameState *gameState ) {
    // Pause current state
    if( !mStates.empty() ) {
        mStates.back()->pause();
    }

    // Store and init the new state
    mStates.push_back( gameState );
    mStates.back()->enter();
}

void GameStateManager::popState() {
    // Cleanup the current state
    if( !mStates.empty() ) {
        mStates.back()->exit();
        mStates.pop_back();
    }

    // Resume previous state
    if( !mStates.empty() ) {
        mStates.back()->resume();
    }
}

void GameStateManager::loginToGame(const std::string &ipAddress, const std::string &playerName)
{
  // FIXME: Kinda ugly initializing playstate here.
  mPlayState = new PlayState(this, mSceneMgr, pGui, ipAddress, playerName);
  changeState(mPlayState);
}

void GameStateManager::requestShutdown( void ) {
    bShutdown = true;
}

  // Keyboard listener
  bool GameStateManager::keyPressed( const ap::ooinput::KeyEvent &e ) {
    // Call keyPressed of current state
    mStates.back()->keyPressed( e );

    return true;
}

  bool GameStateManager::keyReleased( const ap::ooinput::KeyEvent &e ) {
    // Call keyReleased of current state
    mStates.back()->keyReleased( e );

    return true;
}

  // Mouse listener
  bool GameStateManager::mousePressed( const ap::ooinput::MouseClickedEvent &e)
  {
    mStates.back()->mousePressed( e );
	return true; // This should be the last possible place to handle inputs.
  }

  bool GameStateManager::mouseReleased( const ap::ooinput::MouseClickedEvent &e)
  {
    mStates.back()->mouseReleased( e );
	return true; // This should be the last possible place to handle inputs.
  }

  bool GameStateManager::mouseMoved( const ap::ooinput::MouseMovedEvent &e)
  {
    mStates.back()->mouseMoved( e );
	return true; // This should be the last possible place to handle inputs.
  }

} // namespace ap
