//
// Created by timbuntu on 8-5-18.
//

#include <ctime>
#include <iostream>
#include "GameIdleState.h"
#include "../../storage/Variables.h"
#include "../GameManager.h"
#include "../TurtleManager.h"
#include "../../utility/CollisionHandler.h"
#include "GameRunningState.h"
#include "GameStartState.h"

/**
 * Constructor for the GameIdleState
 * In this state the game has not started yet but the game acts as an interactable environment.
 * When a player is in the playing field for long enough then a StartingArea will appear.
 * If the player stands for some time in the StartingArea then the GameManager will transition to the GameRunningState.
 *
 * @param gameManager - The currently active GameManager.
 */
GameIdleState::GameIdleState(GameManager *gameManager) {
    this->gameManager = gameManager;

    // Initialize all of the turtle objects.
    auto *tempTurtleManager = new TurtleManager();
    for (int i = 0; i < Variables::MAX_VISIBLE_TURTLES; i++) {
        tempTurtleManager->createTurtle();
    }

    this->turtleManager = tempTurtleManager;
    this->playerManager = new PlayerManager();

    // Change the state of a subset of the turtles to make the interactive environment more interesting.
    this->turtleManager->diversifyStates();

    // Initialize the GameIdleState.
    init();
}

/**
 * Constructor for the GameIdleState.
 * This constructor is used when transitioning from the GameResetState to the GameIdleState.
 * It takes an existing TurtleManager and PlayerManager to preserve positions of both turtles and players when going
 * back to the idle state.
 *
 * @param gameManger - the currently active GameManager.
 * @param turtleManager - the currently active TurtleManager.
 * @param playerManager - the currently active PlayerManager.
 */
GameIdleState::GameIdleState(GameManager *gameManger, TurtleManager *turtleManager, PlayerManager *playerManager) {
    this->gameManager = gameManger;
    this->turtleManager = turtleManager;
    this->playerManager = playerManager;

    // Initialize the GameIdleState.
    init();
}

/**
 * Method that initializes the GameIdleState.
 *
 * A collisionHandler is created.
 * A Starting area is created.
 * And the number of dead turtles is reset.
 */
void GameIdleState::init() {
    this->collisionHandler = new CollisionHandler(this->playerManager, this->turtleManager,
                                                  new ObstacleManager(), gameManager->getObjectiveAreas());

    // Calculate StartingArea positioning based on the size configured in Variables.
    int width_pixels = Variables::scaleToPixels(Variables::START_AREA_WIDTH);
    int height_pixels = Variables::scaleToPixels(Variables::START_AREA_HEIGHT);
    int x_middle = Variables::PIXEL_WIDTH / 2;

    // Create a StartingArea with the calculated values.
    this->startingArea = new StartingArea(x_middle - width_pixels / 2, x_middle + width_pixels / 2,
                                          Variables::PIXEL_HEIGHT - height_pixels, Variables::PIXEL_HEIGHT);

    // Reset the number of dead turtles, this can be left over from a previously reset game.
    Variables::DEAD_TURTLES = 0;

    // In this state we want turtles to spawn infinitely thus we set this variable to true.
    turtleManager->setInfiniteSpawn(true);
}

/**
 * Updates the state every tick.
 * Timers for checks whether players want to start the game are updated.
 * Interactive entities are also updated to facilitate an interactive environment while the game is not being played.
 *
 * @param inputList - List of input data types.
 */
void GameIdleState::update(std::list<InputData *> inputList) {

    // Force start the game on debug input.
    if (inputList.front()->isSpacePressed()) {
        startGame();
    }

    // If the inputList is not empty (needed for frameTime) then update the timers on the StartingArea object.
    if (!inputList.empty()) {
        startingArea->updateTimers(playerManager->getPlayers(), inputList.front()->getFrameTime());
    }

    // Start the game if a player stood in the StartingArea for long enough.
    if (startingArea->isActivated()) {
        startGame();
    }

    // Update collisions and all game entities.
    collisionHandler->handleCollision();
    turtleManager->update(inputList);
    playerManager->update(inputList);
}

// LCOV_EXCL_START
/**
 * Renders the contents of the state, including the StartingArea.
 * @param renderWrapper - Wrapper for the render window.
 */
void GameIdleState::render(RenderWrapper *renderWrapper) {
    renderWrapper->renderBorders();
    startingArea->render(renderWrapper);
    playerManager->renderPlayers(renderWrapper);
    turtleManager->renderTurtles(renderWrapper);
}
// LCOV_EXCL_STOP

/**
 * Starts the game and resets the needed variables.
 */
void GameIdleState::startGame() {
    if (Variables::SHOW_TUTORIAL) {
        BasicState *oldState = gameManager->getCurrentState();
        gameManager->setCurrentState(new GameStartState(gameManager, playerManager));
        delete oldState;
    } else {
        BasicState *oldState = gameManager->getCurrentState();
        gameManager->setCurrentState(new GameRunningState(gameManager, playerManager));
        delete oldState;
    }
}

/**
 * Getter for the currently active PlayerManager.
 * @return playerManager - the current PlayerManager.
 */
PlayerManager *GameIdleState::getPlayerManager() const {
    return playerManager;
}

/**
 * Getter for the StateIdentifier of this state, this can be used to easily identify which instantiation of BasicState
 * this is. Mainly used for debug draws.
 * @return stateIdentifier - The string that represents the name of this state.
 */
std::string GameIdleState::getStateIdentifier() {
    return "GameIdleState";
}