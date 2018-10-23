//
// Created by timbuntu on 8-5-18.
//

#include "GameManager.h"
#include "gameStates/GameIdleState.h"
#include "../storage/InputData.h"
#include "../rendering/RenderWrapper.h"
#include "../utility/BasicState.h"
#include <list>
#include <iostream>
#include "gameStates/GameRunningState.h"
#include "../storage/Variables.h"
#include "gameStates/GameResetState.h"
#include "../utility/MathUtilities.h"

/**
 * Constructor for the GameManager object.
 */
GameManager::GameManager() {
    createObjectiveAreas();
    currentState = new GameIdleState(this);
}

/**
 * Method for initializing the ObjectiveAreas, more can be added if needed and positions can be adapted to change
 * the level structure.
 */
void GameManager::createObjectiveAreas() {
    double scaledRadius = Variables::scaleToPixels(Variables::OBJECTIVE_RADIUS);

    auto *northWest = new ObjectiveArea(0.0, 0.0, scaledRadius, 225.0);
    auto *northEast = new ObjectiveArea(Variables::PIXEL_WIDTH, 0.0, scaledRadius, 315.0);
    auto *southWest = new ObjectiveArea(0.0, Variables::PIXEL_HEIGHT, scaledRadius, 135.0);
    auto *southEast = new ObjectiveArea(Variables::PIXEL_WIDTH, Variables::PIXEL_HEIGHT, scaledRadius, 45.0);

    this->objectiveAreas.push_back(northWest);
    this->objectiveAreas.push_back(northEast);
    this->objectiveAreas.push_back(southWest);
    this->objectiveAreas.push_back(southEast);

    MathUtilities::setObjectiveAreas(objectiveAreas);
}

/**
 * Called upon each frame to update game logic.
 * @param inputList - The list of input types.
 */
void GameManager::update(std::list<InputData *> inputList) {

    // Toggle the debug draw when J is pressed.
    if (!inputList.empty() && inputList.front()->isJPressed()) {
        Variables::DEBUG_MODE = !Variables::DEBUG_MODE;
    }

    currentState->update(inputList);

    // Set the DEBUG game state variable to the current state identifier for debugging purposes.
    Variables::DEBUG_GAME_STATE = currentState->getStateIdentifier();
}

// LCOV_EXCL_START
/**
 * Orders the current state to render visuals.
 * @param renderWrapper - Wrapper of the render window.
 */
void GameManager::render(RenderWrapper *renderWrapper) {
    currentState->render(renderWrapper);

    list<ObjectiveArea *>::iterator it;
    for (it = objectiveAreas.begin(); it != objectiveAreas.end(); ++it) {
        (*it)->render(renderWrapper);
    }
}
// LCOV_EXCL_STOP

/**
 * Getter for the current GameManager state.
 * @return currentState - The current state of the game.
 */
BasicState *GameManager::getCurrentState() const {
    return currentState;
}

/**
 * Sets the state of the GameManager.
 * @param state - The game state to be set.
 */
void GameManager::setCurrentState(BasicState *currentState) {
    GameManager::currentState = currentState;
}

/**
 * Getter for the PlayerManager that uses dynamic casts to check whether a state is active
 * that maintains a PlayerManager.
 * @return playerManager - a PlayerManager object if a state with a PlayerManager was active. Otherwise a nullptr.
 */
PlayerManager *GameManager::getPlayerManager() const {

    if (dynamic_cast<GameRunningState *>(currentState)) {
        auto *gameRunningState = dynamic_cast<GameRunningState *>(currentState);
        return gameRunningState->getPlayerManager();
    } else if (dynamic_cast<GameResetState *>(currentState)) {
        auto *gameResetState = dynamic_cast<GameResetState *>(currentState);
        return gameResetState->getPlayerManager();
    } else if (dynamic_cast<GameIdleState *>(currentState)) {
        auto *gameIdleState = dynamic_cast<GameIdleState *>(currentState);
        return gameIdleState->getPlayerManager();
    } else {
        return nullptr;
    }
}

/**
 * Getter for the objectiveAreas in the game, this is used by gameStates to construct CollisionHandlers.
 * @return objectiveAreas - the list of ObjectiveAreas that are currently in the game.
 */
list<ObjectiveArea *> GameManager::getObjectiveAreas() const {
    return objectiveAreas;
}