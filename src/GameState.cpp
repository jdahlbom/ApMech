#include "GameState.h"
#include "GameStateManager.h"

namespace ap {

void GameState::initStateManager(GameStateManager *gameStateManager) {
    pGameStateManager = gameStateManager;
}

GameStateManager* GameState::getStateManager() {
  return pGameStateManager;
}

void GameState::requestShutdown( void ) {
    pGameStateManager->requestShutdown();
}

} // namespace ap
