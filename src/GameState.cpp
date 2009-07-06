/**
 * Most of the code was lifted from a wiki-linked example application:
 * http://www.ogre3d.org/wiki/index.php/Using_OIS
 * http://dev.neoharmonix.net/ois-example.zip
 **/

#include "GameState.h"
#include "GameStateManager.h"

namespace ap {

void GameState::initStateManager(GameStateManager *gameStateManager) {
    pGameStateManager = gameStateManager;
}

void GameState::changeState( GameState *state ) {
    pGameStateManager->changeState( state );
}

void GameState::pushState( GameState *state ) {
    pGameStateManager->pushState( state );
}

void GameState::popState( void ) {
    pGameStateManager->popState();
}

void GameState::requestShutdown( void ) {
    pGameStateManager->requestShutdown();
}

} // namespace ap
