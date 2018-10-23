//
// Created by thijmen on 6-6-18.
//

#include <sstream>
#include "TurtleRoamingState.h"
#include "../../../storage/Variables.h"
#include "../../../utility/MathUtilities.h"
#include "TurtleIdleState.h"
#include "../../../utility/CollisionHandler.h"
#include "TurtleObjectiveState.h"
#include "TurtleWalkState.h"

using namespace std;

/**
 * Constructor for the roaming state of the turtle.
 * Turtles go in the roaming state after spawning and when pushed out of the spawning area.
 * When Roaming, turtles wander around the sand area randomly.
 * @param turtle - Turtle to set the state for.
 */
TurtleRoamingState::TurtleRoamingState(Turtle *turtle) {
    this->turtle = turtle;
    this->startRotation = turtle->getDirection();
    this->goalRotation = 0.0;
    this->goalX = 0.0;
    this->goalY = 0.0;
    this->turtle->setReturning(false);
    this->turtle->setIdle(false);
    this->turtle->setRoaming(true);
    this->turtle->setInteractable();
    this->turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_ROAMING_SPEED));
    this->turtle->setMoveTimer(0.0);
}

/**
 * Updates the turtle every tick.
 * @param inputList - The list of input types.
 */
void TurtleRoamingState::update(list<InputData *> inputList) {
    if (!turtle->isReturning()) {
        move(inputList.front());
    }
    updateCounters(inputList.front());

    // Lower the speed if the turtle is against the wall to prevent a cheesy strategy of sliding turtles along
    // the wall.
    if (turtle->isAgainstWall()) {
        this->turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_BORDER_SPEED));
    } else {
        this->turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_ROAMING_SPEED));
    }
}

// LCOV_EXCL_START
/**
 * Renders the state.
 * @param renderWrapper - Wrapper for the render window.
 */
void TurtleRoamingState::render(RenderWrapper *renderWrapper) {
    std::string identifier = getStateIdentifier();
    renderWrapper->renderTurtle(turtle->getX(), turtle->getY(), turtle->getDirection(), 255.0, &identifier,
                                this->turtle->getId());
}
// LCOV_EXCL_STOP

/**
 * Method for acting upon a collision with another turtle.
 * @param other - a reference to the other turtle.
 */
void TurtleRoamingState::onCollide(Turtle &other) {
    // Do nothing.
}

/**
 * Method for acting upon a collision with a player.
 * @param collidedPlayers - a list of players that are currently colliding with the turtle.
 * @param innerHitBox - a boolean that is true if the turtle is colliding with the innerHitBox of the player.
 */
void TurtleRoamingState::onCollide(list<Player *> collidedPlayers, bool innerHitBox) {
    turtle->setColliding(true);

    // Transition to the TurtleWalkState.
    BasicState *oldState2 = turtle->getCurrentState();
    turtle->setCurrentState(new TurtleWalkState(turtle));
    delete oldState2;

    // Initialize the turtle location and direction.
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
}

/**
 * Method for acting upon a collision with an objectiveArea.
 * @param other - a reference to the objectiveArea.
 */
void TurtleRoamingState::onCollide(ObjectiveArea &other) {
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
void TurtleRoamingState::onCollideObstacle() {
    turtle->kill();
}

/**
 * Move method for the turtle RoamingState.
 * If the turtle roams into the spawn location then the state transitions to the TurtleIdleState.
 * @param inputData - Input data to move the turtle with.
 */
void TurtleRoamingState::move(InputData *inputData) {
    double x = turtle->getX() + (turtle->getSpeed() * inputData->getFrameTime() * cos(turtle->getDirection()));
    double y = turtle->getY() + (turtle->getSpeed() * inputData->getFrameTime() * sin(turtle->getDirection()));
    turtle->setPosition(x, y);

    // Check whether the turtle is in spawn, if so then transition to the TurtleIdleState.
    if (MathUtilities::isInSpawn(static_cast<int>(turtle->getX()), static_cast<int>(turtle->getY()))) {
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
void TurtleRoamingState::updateCounters(InputData *inputData) {

    // Decrement the timer.
    turtle->setMoveTimer(turtle->getMoveTimer() - inputData->getFrameTime());
    turtle->setWiggleTimer(turtle->getWiggleTimer() - inputData->getFrameTime());

    // If the WiggleTimer reaches 0, stop wiggling.
    if (turtle->getMoveTimer() <= 0) {
        pair<double, double> location = getRandomLocation(turtle->getX(), turtle->getY());
        this->goalX = location.first;
        this->goalY = location.second;
        startRotation = turtle->getDirection();
        goalRotation = turtle->calculateDirection(goalX, goalY);
        turtle->setWiggleTimer(Variables::WIGGLE_DURATION_ROAMING);
        turtle->setMoveTimer(Variables::MOVE_DURATION_ROAMING);
    }

    // Change the direction based on the linear interpolation between the start and goal direction.
    double wiggleRotation =
            MathUtilities::linearInterpolation(Variables::WIGGLE_DURATION_ROAMING - turtle->getWiggleTimer(),
                                                               Variables::WIGGLE_DURATION_ROAMING, startRotation,
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
void TurtleRoamingState::turn() {
    // Set the WiggleTimer to the WIGGLE_DURATION, used to determine the end of the next wiggle.
    turtle->setWiggleTimer(Variables::WIGGLE_DURATION_ROAMING);

    // Set the goalRotation to an offset to the opposite direction to make the turtle wiggle.
    turtle->setWiggleFactor(turtle->getWiggleFactor() * -1);
    this->startRotation = turtle->getDirection();
    this->goalRotation = startRotation + Variables::WIGGLE_OFFSET_ROAMING * turtle->getWiggleFactor();
}

/**
 * Function that gets a random location around the given point.
 * This is used to determine what location the Turtle will roam to next.
 * If a turtle is too close to an ObjectiveArea then it will not be able to enter it by itself because a direction
 * away from that ObjectiveArea will be chosen.
 *
 * @param currentX - Current x value of the object.
 * @param currentY - Current y value of the object.
 * @return pair<double, double> - The new x and y values.
 */
pair<double, double> TurtleRoamingState::getRandomLocation(double currentX, double currentY) {

    ObjectiveArea closestSea = MathUtilities::calculateClosestObjectiveArea(currentX, currentY);
    double seaX = closestSea.getX();
    double seaY = closestSea.getY();

    // Total distance between turtle and the closes sea is calculated using the pythagorean theorem.
    double distance = MathUtilities::distanceBetweenPoints(seaX, seaY, turtle->getX(), turtle->getY());
    double randRadians;

    if (distance <= Variables::scaleToPixels(Variables::ROAMING_NOGO_RADIUS)) {
        randRadians = closestSea.getSwimDirection() + M_PI;
    } else {
        randRadians = drand48() * 2 * M_PI;
    }

    double newX = Variables::ROAMING_BUFFER * cos(randRadians) + currentX;
    double newY = Variables::ROAMING_BUFFER * sin(randRadians) + currentY;

    return std::make_pair(newX, newY);
}

/**
 * Getter for the StateIdentifier of this state, this can be used to easily identify which instantiation of BasicState
 * this is. Mainly used for debug draws.
 * @return stateIdentifier - The string that represents the name of this state.
 */
std::string TurtleRoamingState::getStateIdentifier() {
    stringstream stream;
    stream << this->turtle->getId() << ": TurtleRoamingState";
    return stream.str();
}