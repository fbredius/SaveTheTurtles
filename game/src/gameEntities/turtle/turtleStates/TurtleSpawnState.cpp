//
// Created by timbuntu on 8-5-18.
//

#include <iostream>
#include "TurtleSpawnState.h"
#include "../Turtle.h"
#include "TurtleWalkState.h"
#include "../../../storage/Variables.h"
#include "TurtleIdleState.h"
#include "TurtleObjectiveState.h"
#include <list>
#include <sstream>

using namespace std;

/**
 * Constructor for the turtle spawn state.
 * @param turtle - Turtle to set the spawn state for.
 */
TurtleSpawnState::TurtleSpawnState(Turtle *turtle) {
    this->turtle = turtle;
    // The time it takes for an egg to become interactable/fully opaque.
    this->timer = Variables::EGG_SPAWN_TIME;
    this->alpha = 0.0;
}

/**
 * Updates the spawn state.
 * @param inputList - The list of input types.
 */
void TurtleSpawnState::update(std::list<InputData *> inputList) {
    InputData *data = inputList.front();
    if (timer > 0) {
        alpha = 255.0 - (timer / Variables::EGG_SPAWN_TIME) * 255.0;
        timer -= data->getFrameTime();
    } else {
        turtle->setInteractable();
    }
}

// LCOV_EXCL_START
/**
 * Renders the state.
 * @param renderWrapper - Wrapper for the render window.
 */
void TurtleSpawnState::render(RenderWrapper *renderWrapper) {
    renderWrapper->renderEgg(turtle->getX(), turtle->getY(), turtle->getDirection(), alpha);
}
// LCOV_EXCL_STOP

/**
 * Method for acting upon a collision with another turtle.
 * @param other - a reference to the other turtle.
 */
void TurtleSpawnState::onCollide(Turtle &other) {
    // Do nothing.
}

/**
 * Method for acting upon a collision with a player.
 * @param other - a reference to the player.
 * @param innerHitBox - a boolean that is true if the turtle is colliding with the innerHitBox of the player.
 */
void TurtleSpawnState::onCollide(list<Player *> collidedPlayers, bool innerHitBox) {
    turtle->hatch();
}

/**
 * Method for acting upon a collision with an objectiveArea.
 * @param other - a reference to the objectiveArea.
 */
void TurtleSpawnState::onCollide(ObjectiveArea &other) {
    // Change the state to the TurtleObjectiveState.
    auto *newState = new TurtleObjectiveState(turtle, other);
    BasicState *oldState = turtle->getCurrentState();
    turtle->setCurrentState(newState);
    delete oldState;

    // Increment score.
    Variables::addScore(1);

    // Update turtle status.
    turtle->setAlive(false);
    turtle->setInObjective(true);
}

/**
 * Method for acting upon a collision with an obstacle.
 */
void TurtleSpawnState::onCollideObstacle() {
    turtle->kill();
}

/**
 * Getter for timer.
 * @return timer - How long before the timer becomes interactable.
 */
double TurtleSpawnState::getTimer() const {
    return timer;
}

/**
 * Getter for alpha.
 * @return alpha - How opaque the turtle is.
 */
double TurtleSpawnState::getAlpha() const {
    return alpha;
}

/**
 * Getter for the StateIdentifier of this state, this can be used to easily identify which instantiation of BasicState
 * this is. Mainly used for debug draws.
 * @return stateIdentifier - The string that represents the name of this state.
 */
std::string TurtleSpawnState::getStateIdentifier() {
    stringstream stream;
    stream << this->turtle->getId() << ": TurtleSpawnState";
    return stream.str();
}