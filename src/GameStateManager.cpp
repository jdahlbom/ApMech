#include <Ogre.h>

/**
 * Most of the code was lifted from a wiki-linked example application:
 * http://www.ogre3d.org/wiki/index.php/Using_OIS
 * http://dev.neoharmonix.net/ois-example.zip
 **/

#include <OgreWindowEventUtilities.h>
#include <OgreCEGUIRenderer.h>

#include <SDL.h>
#include <CEGUI.h>

#include "GameStateManager.h"

#include "GameState.h"
#include "PlayState.h"

namespace ap {

typedef std::map<GameStateManager::GAMESTATE, GameState *> StateMap;
typedef std::pair<GameStateManager::GAMESTATE, GameState *> StatePair;

GameStateManager::GameStateManager( Ogre::Root *root,
                    Ogre::RenderWindow *renderWindow,
                    ooinput::InputSystem *inputSystem,
                    Ogre::SceneManager *sceneManager) :
    mRoot( root ),
    mRenderWindow( renderWindow ),
    mSceneMgr(sceneManager),
    mInputMgr( inputSystem ),
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

    delete mInputMgr;
    mInputMgr = 0;

    delete mPlayState;
    mPlayState  = 0;

    delete mRoot;
    mRoot = 0;
}

void GameStateManager::startGame() {
    mStateMap = new StateMap();
    // Setup states
    //TODO: Annetaan näille luontivaiheessa pointteri manageriin.
    mPlayState  = new PlayState( this, mSceneMgr);

    mStateMap->insert(StatePair(PLAY, mPlayState));

#if 0
    // Setup and configure game
    this->setupResources();
    if( !this->configureGame() ) {
        // If game can't be configured, throw exception and quit application
        throw Ogre::Exception( Ogre::Exception::ERR_INTERNAL_ERROR,
            "Error - Couldn't Configure Renderwindow",
            "Example - Error" );
        return;
    }
#endif

    // Setup input
    mInputMgr->addKeyboardListener( this );

    // Change to first state
    this->changeState( mPlayState );

    // lTimeLastFrame remembers the last time that it was checked
    // We use it to calculate the time since last frame
    unsigned long lTimeLastFrame = 0;

    // Main while-loop
    while( !bShutdown ) {
        // Calculate time since last frame and remember current time for next frame
        unsigned long lTimeCurrentFrame = mRoot->getTimer()->getMilliseconds();
        unsigned long lTimeSinceLastFrame = lTimeCurrentFrame - lTimeLastFrame;
        lTimeLastFrame = lTimeCurrentFrame;

        // Update inputmanager
        mInputMgr->capture();

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

bool GameStateManager::configureGame( void ) {
    // Initialise resources
  //Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    return true;
}

void GameStateManager::setupResources( void ) {
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

} // namespace ap
