//
// Created by maxim on 5-6-18.
//

#include <iostream>
#include "GameResetState.h"
#include "GameIdleState.h"
#include "GameRunningState.h"

using namespace std;

/**
 * Constructor of the reset state.
 * This state can only be reached from the GameRunningState, the existing PlayerManager and TurtleManager
 * are stored in this state to allow for the construction of a GameRunningState when
 * a player enters the playing field during the reset, this will cancel the reset.
 *
 * Furthermore this constructor triggers a reset of all turtles that causes them to start moving back to the spawn.
 *
 * Once all turtles are in the spawn then the GameResetState is complete and it will transition to the GameIdleState.
 *
 * @param gameManager - The current GameManager.
 * @param turtleManager - The current TurtleManager, used to reset the turtles that are in the game.
 * @param playerManager - The current PlayerManager, used to detect new players.
 * @param obstacleManager - The current ObstacleManager, used to maintain and re-spawn obstacles.
 */
GameResetState::GameResetState(GameManager *gameManager, TurtleManager *turtleManager, PlayerManager *playerManager,
                               ObstacleManager *obstacleManager) {
    this->gameManager = gameManager;
    this->turtleManager = turtleManager;
    this->playerManager = playerManager;
    this->obstacleManager = obstacleManager;
    turtleManager->resetTurtles();
    obstacleManager->setActive(true);
}

/**
 * The update method of the GameResetState.
 * It checks whether the turtles have been reset, if this is the case then we switch to another state.
 * @param inputList - Used to update the turtleManager.
 */
void GameResetState::update(std::list<InputData *> inputList) {
    obstacleManager->update(inputList);
    turtleManager->update(inputList);
    playerManager->update(inputList);

    // Cancel the reset if a player enters the field and go back to the GameRunningState.
    if (playerManager->numberOfPlayers() > 0) {
        gameManager->setCurrentState(new GameRunningState(gameManager, turtleManager, playerManager, obstacleManager));
    }

    // If all turtles have returned then start to fade all obstacles, to prevent them from just de-spawning.
    if (turtleManager->allTurtlesReturned()) {
        obstacleManager->fadeAll();
    }

    // When all obstacles were removed, transition to te GameIdleState.
    if (obstacleManager->getObstacles()->empty()) {
        gameManager->setCurrentState(new GameIdleState(gameManager, turtleManager, playerManager));
    }
}

// LCOV_EXCL_START
/**
 * Renders the GUI and the turtles, players are not rendered because there are no players during the reset state.
 */
void GameResetState::render(RenderWrapper *renderWrapper) {
    renderWrapper->renderBorders();
    obstacleManager->render(renderWrapper);
    turtleManager->renderTurtles(renderWrapper);
    renderWrapper->renderGUI();
}
// LCOV_EXCL_STOP

/**
 * Getter for the PlayerManager.
 * @return playerManager - the PlayerManager object that was received from the last GameRunningState.
 */
PlayerManager *GameResetState::getPlayerManager() const {
    return playerManager;
}

/**
 * Getter for the StateIdentifier of this state, this can be used to easily identify which instantiation of BasicState
 * this is. Mainly used for debug draws.
 * @return stateIdentifier - The string that represents the name of this state.
 */
std::string GameResetState::getStateIdentifier() {
    return "GameResetState";
}