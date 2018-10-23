//
// Created by timbuntu on 8-5-18.
//

#include <iostream>
#include "TurtleSpawnState.h"
#include "TurtleReturningState.h"
#include "../Turtle.h"
#include <cmath>
#include "TurtleWalkState.h"
#include "../../../storage/Variables.h"
#include "../../../utility/MathUtilities.h"
#include "TurtleIdleState.h"
#include "TurtleObjectiveState.h"
#include <list>
#include <sstream>

using namespace std;

/**
 * Constructor for the turtle returning state.
 * @param turtle - Turtle to set the returning state for.
 */
TurtleReturningState::TurtleReturningState(Turtle *turtle) {
    this->turtle = turtle;
    this->startRotation = turtle->getDirection();
    this->turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_WALK_SPEED));
    this->returnX = Variables::PIXEL_WIDTH / 2.0;
    this->returnY = Variables::PIXEL_HEIGHT / 2.0;
    this->goalRotation = turtle->calculateDirection(returnX, returnY);
    this->turtle->setWiggleTimer(Variables::WIGGLE_DURATION_RETURNING);
    this->turtle->setReturning(true);
    this->initialRotateFinished = false;
}

/**
 * Updates the return state.
 * @param inputList - The list of input types.
 */
void TurtleReturningState::update(std::list<InputData *> inputList) {
    if (initialRotateFinished) {
        move(inputList.front());
    }
    updateCounters(inputList.front());
}

// LCOV_EXCL_START
/**
 * Renders the state.
 * @param renderWrapper - Wrapper for the render window.
 */
void TurtleReturningState::render(RenderWrapper *renderWrapper) {
    std::string identifier = getStateIdentifier();
    renderWrapper->renderTurtle(turtle->getX(), turtle->getY(), turtle->getDirection(), 255.0, &identifier,
                                this->turtle->getId());
}
// LCOV_EXCL_STOP

/**
 * Method for acting upon a collision with another turtle.
 * @param other - a reference to the other turtle.
 */
void TurtleReturningState::onCollide(Turtle &other) {
    // Do nothing.
}

/**
 * Method for acting upon a collision with a player.
 * @param collidedPlayers - a list of players that are currently colliding with the turtle.
 * @param innerHitBox - a boolean that is true if the turtle is colliding with the innerHitBox of the player.
 */
void TurtleReturningState::onCollide(list<Player *> collidedPlayers, bool innerHitBox) {
    // Do nothing, no interaction is possible.
}

/**
 * Method for acting upon a collision with an objectiveArea.
 * @param other - a reference to the objectiveArea.
 */
void TurtleReturningState::onCollide(ObjectiveArea &other) {
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
void TurtleReturningState::onCollideObstacle() {
    // Do nothing, turtle is immune.
}

/**
 * Move method for the turtle WalkState.
 * @param inputData - Input data to move the turtle with.
 */
void TurtleReturningState::move(InputData *inputData) {
    bool inSpawn = MathUtilities::isInSpawn(static_cast<int>(turtle->getX()), static_cast<int>(turtle->getY()));

    // If the turtle is not in spawn yet we continue moving.
    if (!inSpawn) {
        double x = turtle->getX() + (turtle->getSpeed() * inputData->getFrameTime() * cos(turtle->getDirection()));
        double y = turtle->getY() + (turtle->getSpeed() * inputData->getFrameTime() * sin(turtle->getDirection()));
        turtle->setPosition(x, y);
    } else {
        // Else we transition to the TurtleIdleState because we reached the spawn location.
        BasicState *oldState = turtle->getCurrentState();
        turtle->setCurrentState(new TurtleIdleState(turtle));
        delete oldState;
    }
}

/**
 * Method that maintains counters to determine the rotation of the turtle.
 * After updating the counters, methods are called to act upon those counters and to rotate the turtle.
 * @param inputData - Input data to update the turtle counters with.
 */
void TurtleReturningState::updateCounters(InputData *inputData) {
    // Decrement the timer
    turtle->setWiggleTimer(turtle->getWiggleTimer() - inputData->getFrameTime());

    // If the wiggleTimer ends we turn the turtle by changing the direction slightly in the opposite direction.
    // This causes the turtle to wiggle.
    // In the Returning state we do not move the turtle until the first rotation to the centre is done. This is marked
    // by the initialRotateFinished boolean.
    if (turtle->getWiggleTimer() <= 0) {
        initialRotateFinished = true;
        turn();
    }

    // Change the direction based on the linear interpolation between the start and goal direction.
    double wiggleRotation =
            MathUtilities::linearInterpolation(Variables::WIGGLE_DURATION_RETURNING - turtle->getWiggleTimer(),
                                                               Variables::WIGGLE_DURATION_RETURNING,
                                                               startRotation, goalRotation);
    turtle->setDirection(wiggleRotation);
}

/**
 * Turn the turtle by changing its direction in the opposite direction.
 * This causes the turtle to wiggle.
 * startRotation, goalRotation and the wiggleTimer are set accordingly to allow a smooth rotation.
 */
void TurtleReturningState::turn() {
    // Set the WiggleTimer to the WIGGLE_DURATION, used to determine the end of the next wiggle.
    turtle->setWiggleTimer(Variables::WIGGLE_DURATION_RETURNING);

    turtle->setWiggleFactor(turtle->getWiggleFactor() * -1);
    this->startRotation = turtle->getDirection();
    this->goalRotation = startRotation + Variables::WIGGLE_OFFSET_RETURNING * turtle->getWiggleFactor();
}

/**
 * Getter for returnX.
 * @return returnX - The x coordinate of the goal location.
 */
double TurtleReturningState::getReturnX() const {
    return returnX;
}

/**
 * Getter for returnY.
 * @return returnY - The y coordinate of the goal location.
 */
double TurtleReturningState::getReturnY() const {
    return returnY;
}

/**
 * Setter for returnX.
 * @param returnX - The x coordinate of the goal.
 */
void TurtleReturningState::setReturnX(double returnX) {
    TurtleReturningState::returnX = returnX;
}

/**
 * Setter for returnY.
 * @param returnY - The y coordinate of the goal.
 */
void TurtleReturningState::setReturnY(double returnY) {
    TurtleReturningState::returnY = returnY;
}

/**
 * Getter for the StateIdentifier of this state, this can be used to easily identify which instantiation of BasicState
 * this is. Mainly used for debug draws.
 * @return stateIdentifier - The string that represents the name of this state.
 */
std::string TurtleReturningState::getStateIdentifier() {
    stringstream stream;
    stream << this->turtle->getId() << ": TurtleReturningState";
    return stream.str();
}