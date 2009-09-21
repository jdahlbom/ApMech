#include <Ogre.h>

#include <OgreWindowEventUtilities.h>

#include <SDL.h>

#include "GameStateManager.h"
#include "Gui.h"

#include "GameState.h"
#include "PlayState.h"
#include "functions.h"

namespace ap {

typedef std::map<GameStateManager::GAMESTATE, GameState *> StateMap;
typedef std::pair<GameStateManager::GAMESTATE, GameState *> StatePair;

GameStateManager::GameStateManager(
		    Ogre::Root 	 *root,
                    ooinput::InputSystem *inputSystem,
                    Ogre::SceneManager *sceneManager,
		    ap::Gui *gui) :
    mRoot(root),
    mSceneMgr(sceneManager),
    pGui(gui),
    pInputMgr( inputSystem ),
    mPlayState( 0 ),
    bShutdown( false ) {}

GameStateManager::~GameStateManager( void )
{
    // Clean up all the states
    while( !mStates.empty() ) {
        mStates.back()->exit();
        mStates.pop_back();
    }

    delete mStateMap;
    mStateMap = 0;

    delete mPlayState;
    mPlayState  = 0;
}

void GameStateManager::startGame() {
    mStateMap = new StateMap();
    // Setup states
    mPlayState  = new PlayState( this, mSceneMgr, pGui);

    mStateMap->insert(StatePair(PLAY, mPlayState));

    // Setup input
    pInputMgr->addKeyboardListener( this );
    pInputMgr->addMouseListener( this );

    // Change to first state
    this->changeState( mPlayState );

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
            usleep( (10.0f - lTimeSinceLastFrame) * 1000.0f);
        }
    }
}

GameState* GameStateManager::getStatePtr(GAMESTATE state) {
    StateMap::iterator end = mStateMap->end();
    StateMap::iterator currentIterator = mStateMap->find(state);

    if( currentIterator == end ) {
      throw std::runtime_error("Managed to ask for a non-existent state object, aborting!");
    }

    return dynamic_cast<GameState*>(currentIterator->second);

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
  }
  
  bool GameStateManager::mouseReleased( const ap::ooinput::MouseClickedEvent &e) 
  {
    mStates.back()->mouseReleased( e );
  }

  bool GameStateManager::mouseMoved( const ap::ooinput::MouseMovedEvent &e) 
  {
    mStates.back()->mouseMoved( e );
  }

} // namespace ap
