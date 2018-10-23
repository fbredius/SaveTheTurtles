//
// Created by maxim on 17-6-18.
//

#include "GameStartState.h"
#include "../../storage/Variables.h"
#include "GameRunningState.h"

/**
 * Constructor for a GameStartState.
 * This state is a bridging state between the GameIdleState and the GameRunningState.
 * The purpose of this state is to display an explanation screen before the game starts.
 * @param gameManager - The currently active GameManager.
 * @param playerManager - The currently active PlayerManager.
 */
GameStartState::GameStartState(GameManager *gameManager, PlayerManager *playerManager) {
    this->gameManager = gameManager;
    this->playerManager = playerManager;
    this->timer = Variables::START_SCREEN_TIME;
}

/**
 * The update method that checks whether the duration of the screen is over by decrementing a timer.
 * If the timer is over, the runGame method is called.
 * @param inputList - An inputList used for retrieving the frameTime to decrement the counter.
 */
void GameStartState::update(std::list<InputData *> inputList) {
    if (inputList.empty()) {
        return;
    }

    timer -= inputList.front()->getFrameTime();

    if (timer <= 0) {
        runGame();
    }
}

// LCOV_EXCL_START
/**
 * The render method that only renders the explanation screen.
 * @param renderWrapper - The RenderWrapper that is used to render the visual.
 */
void GameStartState::render(RenderWrapper *renderWrapper) {
    renderWrapper->renderExplainScreen();
}
// LCOV_EXCL_STOP

/**
 * Method that runs the game by transitioning to the GameRunningState and by resetting Variables.
 */
void GameStartState::runGame() {
    BasicState *oldState = gameManager->getCurrentState();
    gameManager->setCurrentState(new GameRunningState(gameManager, playerManager));
    delete oldState;
    Variables::DEAD_TURTLES = 0;
    Variables::resetScore();
}

/**
 * Getter for the StateIdentifier of this state, this can be used to easily identify which instantiation of BasicState
 * this is. Mainly used for debug draws.
 * @return stateIdentifier - The string that represents the name of this state.
 */
std::string GameStartState::getStateIdentifier() {
    return "GameStartState";
}