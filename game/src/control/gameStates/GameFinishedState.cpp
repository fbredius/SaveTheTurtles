//
// Created by maxim on 17-5-18.
//

#include "GameFinishedState.h"
#include "GameIdleState.h"
#include "../../storage/Variables.h"
#include <sstream>
#include <iostream>

using namespace std;

/**
 * Constructor for the game state.
 * @param machine - Current machine.
 */
GameFinishedState::GameFinishedState(GameManager *gameManager) {
    this->gameManager = gameManager;
    this->fadeFinished = false;
    this->hasFinished = false;
    this->duration = Variables::END_SCREEN_TIME;
}

/**
 * Updates the GameFinishedState.
 * @param inputList - The list of input types.
 */
void GameFinishedState::update(std::list<InputData *> inputList) {

    // When space is pressed, reset the score, the dead turtles and set the game to GameIdleState.
    if (inputList.front()->isSpacePressed()) {
        resetGame();
    }

    // Decrement the timer for automatic resetting.
    if (!inputList.empty()) {
        duration -= inputList.front()->getFrameTime();
    }

    // Reset the game after a predetermined time.
    if (duration <= 0.0) {
        resetGame();
    }
}

// LCOV_EXCL_START
/**
 * Renders the state.
 * Once the alpha value reaches zero, the fade in has finished
 * and the code shouldn't be executed anymore.
 * @param renderWrapper - Wrapper for the render window.
 */
void GameFinishedState::render(RenderWrapper *renderWrapper) {

    if (hasFinished && !fadeFinished) {

        renderWrapper->fadeOut();

        // If the fadeAlpha value has reached 255, the fade has been completed and the state can switch.
        // Also reset the frame time in renderWrapper.
        if (renderWrapper->getFadeAlpha() >= 255.0) {
            fadeFinished = true;
            renderWrapper->resetFadeVariables();
            renderWrapper->setFrameTime(0.0f);
        }
    }

    if (Variables::getScore() >= Variables::SCORE_WIN_THRESHOLD) {
        renderWrapper->renderWinScreen();
    } else {
        renderWrapper->renderLoseScreen();
    }

    if (!fadeFinished) {
        renderWrapper->fadeIn();
        if (renderWrapper->getFadeAlpha() <= 0) {
            fadeFinished = true;
        }
    }
}
// LCOV_EXCL_STOP

/**
 * Reset score and change the state back to the GameIdleState.
 * Also resets the number of dead turtles.
 */
void GameFinishedState::resetGame() {
    Variables::setScore(0);
    Variables::DEAD_TURTLES = 0;
    BasicState *oldState = gameManager->getCurrentState();
    this->gameManager->setCurrentState(new GameIdleState(this->gameManager));
    delete oldState;
}

/**
 * Getter for the StateIdentifier of this state, this can be used to easily identify which instantiation of BasicState
 * this is. Mainly used for debug draws.
 * @return stateIdentifier - The string that represents the name of this state.
 */
std::string GameFinishedState::getStateIdentifier() {
    return "GameFinishedState";
}