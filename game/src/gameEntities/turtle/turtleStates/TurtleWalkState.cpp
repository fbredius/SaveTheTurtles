//
// Created by timbuntu on 8-5-18.
//

#include <algorithm>
#include <iostream>
#include <cmath>
#include "../Turtle.h"
#include "TurtleWalkState.h"
#include "../../../storage/Variables.h"
#include "TurtleSpawnState.h"
#include "TurtleIdleState.h"
#include "../../../utility/MathUtilities.h"
#include "TurtleObjectiveState.h"
#include "../../../utility/CollisionHandler.h"
#include "TurtleRoamingState.h"
#include <list>
#include <sstream>

using namespace std;

/**
 * Constructor for the walking state of the turtle.
 * @param turtle - Turtle to set the state for.
 */
TurtleWalkState::TurtleWalkState(Turtle *turtle) {
    this->turtle = turtle;
    this->startRotation = turtle->getDirection();
    this->goalRotation = 0.0;
    this->turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_WALK_SPEED));
    this->turtle->setWiggleTimer(0.0);
    this->turtle->setMoveTimer(Variables::MOVE_DURATION_WALK);
    this->turtle->setReturning(false);
    this->turtle->setIdle(false);
    this->turtle->setRoaming(false);
    this->turtle->setInteractable();

    // Set resetting to false in case the turtle was interrupted from a natural reset.
    this->turtle->setResetting(false);

    turn();
}

/**
 * Update method for the turtle WalkState.
 * @param inputList - The list of input types.
 */
void TurtleWalkState::update(list<InputData *> inputList) {
    updateCounters(inputList.front());
    move(inputList.front());

    // Lower the speed if the turtle is against the wall to prevent a cheesy strategy of sliding turtles along
    // the wall.
    if (turtle->isAgainstWall()) {
        this->turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_BORDER_SPEED));
    } else {
        this->turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_WALK_SPEED));
    }
}

// LCOV_EXCL_START
/**
 * Render method for the turtle WalkState.
 * @param renderWrapper - Wrapper for the render window.
 */
void TurtleWalkState::render(RenderWrapper *renderWrapper) {
    std::string identifier = getStateIdentifier();
    renderWrapper->renderTurtle(turtle->getX(), turtle->getY(), turtle->getDirection(), 255.0, &identifier,
                                this->turtle->getId());
}
// LCOV_EXCL_STOP

/**
 * Method for acting upon a collision with another turtle.
 * @param other - a reference to the other turtle.
 */
void TurtleWalkState::onCollide(Turtle &other) {
    // Do nothing.
}

/**
 * Method for acting upon a collision with a player.
 * @param other - a reference to the player.
 * @param innerHitBox - a boolean that is true if the turtle is colliding with the innerHitBox of the player.
 */
void TurtleWalkState::onCollide(list<Player *> collidedPlayers, bool innerHitBox) {
    turtle->setColliding(true);

    // Initialize.
    double turtleX = turtle->getX();
    double turtleY = turtle->getY();
    double totalDirection = 0.0;

    // Sum the direction change that each player enforces on the turtle.
    for (Player *player : collidedPlayers) {
        double playerX = player->getX();
        double playerY = player->getY();
        double direction = turtle->calculateDirection(playerX + 2 * (turtleX - playerX),
                                                      playerY + 2 * (turtleY - playerY));

        totalDirection += direction;
    }

    // Get the average direction to simulate a resulting force calculation.
    double averagedDirection = totalDirection / collidedPlayers.size();

    // Set the calculated directions.
    this->startRotation = turtle->getDirection();
    this->goalRotation = averagedDirection;
    this->turtle->setWiggleTimer(Variables::INTERACTION_ROTATE_TIME);

    // Adapt the speed while the turtle is walking depending on which hitBox the turtle is hitting.
    if (turtle->isAgainstWall()) {
        // Lower the speed if the turtle is against the wall to prevent a cheesy strategy of sliding turtles along
        // the wall.
        this->turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_BORDER_SPEED));
    } else if (innerHitBox) {
        turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_FORCE_SPEED));
    } else {
        turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_WALK_SPEED));
    }
}

/**
 * Method for acting upon a collision with an objectiveArea.
 * @param other - a reference to the objectiveArea.
 */
void TurtleWalkState::onCollide(ObjectiveArea &other) {

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
void TurtleWalkState::onCollideObstacle() {
    turtle->kill();
}

/**
 * Move method for the turtle WalkState.
 * @param inputData - Input data to move the turtle with.
 */
void TurtleWalkState::move(InputData *inputData) {
    double x = turtle->getX() + (turtle->getSpeed() * inputData->getFrameTime() * cos(turtle->getDirection()));
    double y = turtle->getY() + (turtle->getSpeed() * inputData->getFrameTime() * sin(turtle->getDirection()));
    turtle->setPosition(x, y);
}

/**
 * Method that maintains counters to determine the rotation of the turtle.
 * After updating the counters, methods are called to act upon those counters and to rotate the turtle.
 * @param inputData - Input data to update the turtle counters with.
 */
void TurtleWalkState::updateCounters(InputData *inputData) {

    // Decrement the timer.
    turtle->setMoveTimer(turtle->getMoveTimer() - inputData->getFrameTime());
    turtle->setWiggleTimer(turtle->getWiggleTimer() - inputData->getFrameTime());

    if (turtle->getMoveTimer() <= 0.0 && turtle->getWiggleTimer() <= 0.0) {
        transitionState();
    }

    // Change the direction based on the linear interpolation between the start and goal direction.
    double wiggleRotation =
            MathUtilities::linearInterpolation(Variables::WIGGLE_DURATION_WALK - turtle->getWiggleTimer(),
                                               Variables::WIGGLE_DURATION_WALK, startRotation, goalRotation);
    turtle->setDirection(wiggleRotation);

    // If the wiggleTimer ends we turn the turtle by changing the direction slightly in the opposite direction.
    // This causes the turtle to wiggle.
    if (turtle->getWiggleTimer() <= 0.0) {
        turn();
    }
}

void TurtleWalkState::transitionState() {

    if (MathUtilities::isInSpawn(static_cast<int>(turtle->getX()), static_cast<int>(turtle->getY()))) {
        // Transition to the TurtleIdleState when the turtle is in the spawn.
        BasicState *oldState = turtle->getCurrentState();
        turtle->setCurrentState(new TurtleIdleState(turtle));
        delete oldState;
    } else {
        // Transition to the TurtleRoamingState when the turtle is not in the spawn.
        BasicState *oldState = turtle->getCurrentState();
        turtle->setCurrentState(new TurtleRoamingState(turtle));
        delete oldState;
    }
}

/**
 * Turn the turtle by changing its direction in the opposite direction.
 * This causes the turtle to wiggle.
 * startRotation, goalRotation and the wiggleTimer are set accordingly to allow a smooth rotation.
 */
void TurtleWalkState::turn() {
    // Set the WiggleTimer to the WIGGLE_DURATION, used to determine the end of the next wiggle.
    turtle->setWiggleTimer(Variables::WIGGLE_DURATION_WALK);

    // Set the goalRotation to an offset to the opposite direction to make the turtle wiggle.
    turtle->setWiggleFactor(turtle->getWiggleFactor() * -1.0);
    this->startRotation = turtle->getDirection();
    this->goalRotation = startRotation + Variables::WIGGLE_OFFSET_WALK * turtle->getWiggleFactor();
}

/**
 * Getter for the StateIdentifier of this state, this can be used to easily identify which instantiation of BasicState
 * this is. Mainly used for debug draws.
 * @return stateIdentifier - The string that represents the name of this state.
 */
std::string TurtleWalkState::getStateIdentifier() {
    stringstream stream;
    stream << this->turtle->getId() << ": TurtleWalkState";
    return stream.str();
}