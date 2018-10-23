//
// Created by timbuntu on 8-5-18.
//

#include "../GameManager.h"
#include "GameRunningState.h"
#include "../../storage/Variables.h"
#include "../../utility/CollisionHandler.h"
#include "../../storage/HitBox.h"
#include "GameFinishedState.h"
#include "GameResetState.h"
#include "../../gameEntities/turtle/turtleStates/TurtleWalkState.h"
#include "../../utility/MathUtilities.h"
#include <sstream>
#include <cmath>
#include <iostream>

using namespace std;

/**
 * Constructor for the GameRunningState.
 * This constructor is used when a new game is started.
 *
 * @param gameManager - The current GameManager.
 */
GameRunningState::GameRunningState(GameManager *gameManager) {
    this->gameManager = gameManager;

    // Initialize all of the turtle objects
    auto *tempTurtleManager = new TurtleManager();
    for (int i = 0; i < Variables::MAX_VISIBLE_TURTLES; i++) {
        tempTurtleManager->createTurtle();
    }

    this->turtleManager = tempTurtleManager;
    this->playerManager = new PlayerManager();
    this->obstacleManager = new ObstacleManager();
    init();
}

/**
 * Constructor for the GameRunningState.
 * This is used when the PlayerManager was already created in the interactive environment.
 *
 * @param gameManager - The current GameManager.
 * @param playerManager - The current PlayerManager.
 */
GameRunningState::GameRunningState(GameManager *gameManager, PlayerManager *playerManager) {
    this->gameManager = gameManager;

    // Initialize all of the turtle objects
    auto *tempTurtleManager = new TurtleManager();
    for (int i = 0; i < Variables::MAX_VISIBLE_TURTLES; i++) {
        tempTurtleManager->createTurtle();
    }

    this->turtleManager = tempTurtleManager;
    this->playerManager = playerManager;
    this->obstacleManager = new ObstacleManager();
    init();
}

/**
 * Constructor for the GameRunningState.
 * This is used when the PlayerManager and TurtleManager were already created in the interactive environment.
 *
 * @param gameManager - The current GameManager.
 * @param turtleManager - The current TurtleManager.
 * @param playerManager - The current PlayerManager.
 */
GameRunningState::GameRunningState(GameManager *gameManager, TurtleManager *turtleManager,
                                   PlayerManager *playerManager) {
    this->gameManager = gameManager;
    this->turtleManager = turtleManager;
    this->playerManager = playerManager;
    this->obstacleManager = new ObstacleManager();
    init();
}

/**
 * Constructor for the GameRunningState.
 * This is used when transitioning from the GameResetState. The already active TurtleManager, PlayerManager and
 * Obstacles are preserved using this constructor.
 *
 * @param gameManager - The current GameManager.
 * @param turtleManager - The current TurtleManager.
 * @param playerManager - The current PlayerManager.
 * @param obstacleManager - The current set of obstacles.
 */
GameRunningState::GameRunningState(GameManager *gameManager, TurtleManager *turtleManager,
                                   PlayerManager *playerManager, ObstacleManager *obstacleManager) {
    this->gameManager = gameManager;
    this->turtleManager = turtleManager;
    this->playerManager = playerManager;
    this->obstacleManager = obstacleManager;
    init();
}

/**
 * Default empty constructor for the GameRunningState.
 */
GameRunningState::GameRunningState() {

    this->gameManager = nullptr;

    // Initialize all of the turtle objects
    auto *tempTurtleManager = new TurtleManager();
    for (int i = 0; i < Variables::MAX_VISIBLE_TURTLES; i++) {
        tempTurtleManager->createTurtle();
    }

    this->turtleManager = tempTurtleManager;
    this->playerManager = new PlayerManager();
    this->obstacleManager = new ObstacleManager();
    init();
}

/**
 * Initialize private variables to starting values.
 * Also initializes obstacles and the collision handler.
 */
void GameRunningState::init() {

    this->collisionHandler = new CollisionHandler(this->playerManager, this->turtleManager, this->obstacleManager,
                                                  gameManager->getObjectiveAreas());

    // The fade has not finished yet.
    fadeFinished = false;

    // Assume that the game is being played, thus activity timers are reset.
    noActivity = false;
    noActivityTimer = Variables::NO_ACTIVITY_TIMEOUT;

    // Every time the game starts, arrows need to be rendered. Once this is done, this is set
    // to false, so it will not render again.
    this->arrowsRendered = false;

    // Activate the obstacleManager hitBoxes.
    obstacleManager->setActive(true);
    obstacleManager->setTurtleManager(this->turtleManager);
}

/**
 * Updates the GameRunningState.
 * @param inputList - The list of input types.
 */
void GameRunningState::update(std::list<InputData *> inputList) {

    // If the fade to GameFinishedState has finished, the state can change.
    if (fadeFinished) {
        BasicState *oldState = gameManager->getCurrentState();
        this->gameManager->setCurrentState(new GameFinishedState(this->gameManager));
        delete oldState;
    }

    // If the game has been won, the fading can start.
    if (Variables::TOTAL_TURTLES == Variables::getScore() + Variables::DEAD_TURTLES
        || (turtleManager->getLeftToSpawn() == 0 && turtleManager->getTurtles().empty())) {

        // Instant transition because fading is temporarily disabled.
        gameManager->setCurrentState(new GameFinishedState(gameManager));
        static_cast<GameFinishedState *>(gameManager->getCurrentState())->hasFinished = true;

    }

    // Run the detectNonActivity method to determine whether the game should reset or not.
    detectNonActivity(inputList);

    // Update collisions and all game entities.
    collisionHandler->handleCollision();
    obstacleManager->update(inputList);
    turtleManager->update(inputList);
    playerManager->update(inputList);
}

// LCOV_EXCL_START
/**
 * Renders the GameRunningState.
 * @param renderWrapper - Wrapper for the render window.
 */
void GameRunningState::render(RenderWrapper *renderWrapper) {
    if (!arrowsRendered) {
        renderWrapper->startArrowAnimations();
        arrowsRendered = true;
    }
    renderWrapper->renderBorders();
    obstacleManager->render(renderWrapper);
    playerManager->renderPlayers(renderWrapper);
    turtleManager->renderTurtles(renderWrapper);
    renderWrapper->renderGUI();
}
// LCOV_EXCL_STOP

/**
 * Getter for the player manager of the GameRunningState.
 * @return playerManager - The current player manager.
 */
PlayerManager *GameRunningState::getPlayerManager() {
    return GameRunningState::playerManager;
}

/**
 * Getter for the turtle manager of the GameRunningState.
 * @return turtleManager - The current turtle manager.
 */
TurtleManager *GameRunningState::getTurtleManager() {
    return GameRunningState::turtleManager;
}

/**
 * Getter for the fade finished variable of the GameRunningState.
 * @return fadeFinished - Denotes whether the fade has finished or not.
 */
bool GameRunningState::isFadeFinished() const {
    return fadeFinished;
}

/**
 * Method for detecting non activity.
 * If there are no players in the playing field anymore then a timer is started.
 * If this timer ends before players enter the field then the state switches to the GameResetState which will
 * gradually move back all turtles back to the spawn.
 *
 * @param inputList - a list of InputData used for decrementing the timers.
 */
void GameRunningState::detectNonActivity(std::list<InputData *> inputList) {

    // If there are no players in the playing field, start or update the no activity timer.
    if (playerManager->numberOfPlayers() == 0) {

        // If this is the first time that non activity was detected then start the timer.
        if (!noActivity) {
            noActivity = true;
            noActivityTimer = Variables::NO_ACTIVITY_TIMEOUT;
        }

        // Update the timer.
        if (!inputList.empty()) {
            noActivityTimer -= inputList.front()->getFrameTime();
        }

        // Go to the reset state if there was no activity for as long as the NO_ACTIVITY_TIMEOUT.
        if (noActivityTimer <= 0.0) {
            this->gameManager->setCurrentState(new GameResetState(this->gameManager, this->turtleManager, this->playerManager, this->obstacleManager));
        }

    } else if (noActivity) {
        // If there are players and noActivity is still true,
        // put it back to false because the game is being played again.
        noActivity = false;
    }

}

/**
 * Getter for the StateIdentifier of this state, this can be used to easily identify which instantiation of BasicState
 * this is. Mainly used for debug draws.
 * @return stateIdentifier - The string that represents the name of this state.
 */
string GameRunningState::getStateIdentifier() {
    return "GameRunningState";
}