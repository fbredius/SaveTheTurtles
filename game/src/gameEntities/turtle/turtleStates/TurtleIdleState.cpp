//
// Created by mariette on 25-5-18.
//

#include <iostream>
#include <sstream>
#include "../Turtle.h"
#include "TurtleIdleState.h"
#include "../../../storage/Variables.h"
#include "TurtleWalkState.h"
#include "../../../utility/MathUtilities.h"
#include "TurtleObjectiveState.h"

using namespace std;

/**
 * Constructor for the TurtleIdleState.
 * Turtles go in the idle state after spawning and when pushed back to the spawning area.
 * When Idle, turtles wander around the spawn area semi randomly.
 * @param turtle - Turtle to set the state for.
 */
TurtleIdleState::TurtleIdleState(Turtle *turtle) {
    this->turtle = turtle;
    this->startRotation = turtle->getDirection();
    this->goalRotation = 0.0;
    this->goalX = 0.0;
    this->goalY = 0.0;
    this->turtle->setReturning(false);
    this->turtle->setIdle(true);
    this->turtle->setInteractable();
    this->turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_IDLE_SPEED));
    this->turtle->setMoveTimer(0.0);
    changeGoalLocation();
}

/**
 * Updates the turtle every tick.
 * @param inputList - The list of input types.
 */
void TurtleIdleState::update(list<InputData *> inputList) {
    if (!turtle->isReturning()) {
        move(inputList.front());
    }
    updateCounters(inputList.front());

    // Lower the speed if the turtle is against the wall to prevent a cheesy strategy of sliding turtles along
    // the wall.
    if (turtle->isAgainstWall()) {
        this->turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_BORDER_SPEED));
    } else {
        this->turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_IDLE_SPEED));
    }

}

// LCOV_EXCL_START
/**
 * Renders the state.
 * @param renderWrapper - Wrapper for the render window.
 */
void TurtleIdleState::render(RenderWrapper *renderWrapper) {
    std::string identifier = getStateIdentifier();
    renderWrapper->renderTurtle(turtle->getX(), turtle->getY(), turtle->getDirection(), 255.0, &identifier,
                                this->turtle->getId());
}
// LCOV_EXCL_STOP

/**
 * Method for acting upon a collision with another turtle.
 * @param other - a reference to the other turtle.
 */
void TurtleIdleState::onCollide(Turtle &other) {
    // Do nothing.
}

/**
 * Method for acting upon a collision with a player.
 * @param collidedPlayers - a list of players that are currently colliding with the turtle.
 * @param innerHitBox - a boolean that is true if the turtle is colliding with the innerHitBox of the player.
 */
void TurtleIdleState::onCollide(list<Player *> collidedPlayers, bool innerHitBox) {
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

    // If the innerHitBox is hit, increase the speed to make the turtle move away from under the player.
    if (innerHitBox) {
        turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_FORCE_SPEED));
    }

    // Transition to the TurtleWalkState.
    BasicState *oldState2 = turtle->getCurrentState();
    turtle->setCurrentState(new TurtleWalkState(turtle));
    delete oldState2;
}

/**
 * Method for acting upon a collision with an objectiveArea.
 * @param other - a reference to the objectiveArea.
 */
void TurtleIdleState::onCollide(ObjectiveArea &other) {
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
void TurtleIdleState::onCollideObstacle() {
    turtle->kill();
}

/**
 * Move method for the turtle IdleState.
 * @param inputData - Input data to move the turtle with.
 */
void TurtleIdleState::move(InputData *inputData) {
    double x = turtle->getX() + (turtle->getSpeed() * inputData->getFrameTime() * cos(turtle->getDirection()));
    double y = turtle->getY() + (turtle->getSpeed() * inputData->getFrameTime() * sin(turtle->getDirection()));
    turtle->setPosition(x, y);
}

/**
 * Method that maintains counters to determine the rotation of the turtle.
 * After updating the counters, methods are called to act upon those counters and to rotate the turtle.
 * @param inputData - Input data to update the turtle counters with.
 */
void TurtleIdleState::updateCounters(InputData *inputData) {

    // Decrement the timer.
    turtle->setWiggleTimer(turtle->getWiggleTimer() - inputData->getFrameTime());

    // If the turtle is close enough to the goal then change the goal location.
    double distance = MathUtilities::distanceBetweenPoints(turtle->getX(), turtle->getY(), goalX, goalY);
    if (distance <= 60) {
        changeGoalLocation();
    }

    // Change the direction based on the linear interpolation between the start and goal direction.
    double wiggleRotation =
            MathUtilities::linearInterpolation(Variables::WIGGLE_DURATION_IDLE - turtle->getWiggleTimer(),
                                               Variables::WIGGLE_DURATION_IDLE, startRotation,
                                               goalRotation);
    turtle->setDirection(wiggleRotation);

    // If the wiggleTimer ends we turn the turtle by changing the direction slightly in the opposite direction.
    // This causes the turtle to wiggle.
    if (turtle->getWiggleTimer() <= 0) {
        turn();
    }
}

/**
 * Turn the turtle by changing its direction in the opposite direction.
 * This causes the turtle to wiggle.
 * startRotation, goalRotation and the wiggleTimer are set accordingly to allow a smooth rotation.
 */
void TurtleIdleState::turn() {
    // Set the WiggleTimer to the WIGGLE_DURATION, used to determine the end of the next wiggle.
    turtle->setWiggleTimer(Variables::WIGGLE_DURATION_IDLE);

    // Set the goalRotation to an offset to the opposite direction to make the turtle wiggle.
    turtle->setWiggleFactor(turtle->getWiggleFactor() * -1);
    this->startRotation = turtle->getDirection();
    this->goalRotation = startRotation + Variables::WIGGLE_OFFSET_IDLE * turtle->getWiggleFactor();
}

/**
 * Method for changing the goalLocation. The goalLocation is set to a random location within the spawnArea.
 * The startRotation, goalRotation and wiggleTimer are reset accordingly.
 */
void TurtleIdleState::changeGoalLocation() {
    pair<double, double> location = MathUtilities::randomSpawnLocation();
    this->goalX = location.first;
    this->goalY = location.second;
    startRotation = turtle->getDirection();
    goalRotation = turtle->calculateDirection(goalX, goalY);
    turtle->setWiggleTimer(Variables::WIGGLE_DURATION_IDLE);
}

/**
 * Getter for the StateIdentifier of this state, this can be used to easily identify which instantiation of BasicState
 * this is. Mainly used for debug draws.
 * @return stateIdentifier - The string that represents the name of this state.
 */
std::string TurtleIdleState::getStateIdentifier() {
    stringstream stream;
    stream << this->turtle->getId() << ": TurtleIdleState";
    return stream.str();
}