//
// Created by thijmen on 29-5-18.
//

#include <sstream>
#include <regex>
#include "TurtleObjectiveState.h"
#include "../../../utility/MathUtilities.h"
#include "../../../storage/Variables.h"
#include "TurtleWalkState.h"
#include "../Turtle.h"

/**
 * Constructor for the turtle objective state.
 * The turtle is in this state when the player has pushed said turtle into any of the four objective areas.
 *
 * @param turtle - Turtle to set the objective state for.
 * @param ObjectiveArea - The ObjectiveArea that the turtle is going into.
 */
TurtleObjectiveState::TurtleObjectiveState(Turtle *turtle, ObjectiveArea objectiveArea) : objectiveArea(objectiveArea) {
    this->turtle = turtle;
    this->startRotation = turtle->getDirection();
    this->goalRotation = turtle->calculateDirection(objectiveArea.getSwimDestination().x,
                                                    objectiveArea.getSwimDestination().y);
    this->turtle->setWiggleTimer(Variables::WIGGLE_DURATION_OBJECTIVE);
    this->turtle->setSpeed(Variables::scaleToPixels(Variables::TURTLE_OBJECTIVE_SPEED));
    this->animationPlayed = false;
}

/**
 * Updates the objective state.
 * @param inputList - The list of input types.
 */
void TurtleObjectiveState::update(std::list<InputData *> inputList) {
    move(inputList.front());
    updateCounters(inputList.front());
}

// LCOV_EXCL_START
/**
 * Renders the state.
 * @param renderWrapper - Wrapper for the render window.
 */
void TurtleObjectiveState::render(RenderWrapper *renderWrapper) {
    if (!animationPlayed) {
        renderWrapper->startScoreAnimation(turtle->getX(), turtle->getY());
        animationPlayed = true;
    }
    std::string identifier = getStateIdentifier();
    double alpha = calculateAlpha();
    renderWrapper->renderTurtle(turtle->getX(), turtle->getY(), turtle->getDirection(), alpha, &identifier,
                                this->turtle->getId());
}
// LCOV_EXCL_STOP

/**
 * Method for acting upon a collision with another turtle.
 * @param other - a reference to the other turtle.
 */
void TurtleObjectiveState::onCollide(Turtle &other) {
    // do nothing.
}

/**
 * Method for acting upon a collision with a player.
 * @param collidedPlayers - a list of players that are currently colliding with the turtle.
 * @param innerHitBox - a boolean that is true if the turtle is colliding with the innerHitBox of the player.
 */
void TurtleObjectiveState::onCollide(list<Player *> collidedPlayers, bool innerHitBox) {
    // Do nothing, no interaction is possible.
}

/**
 * Method for acting upon a collision with an objectiveArea.
 * @param other - a reference to the objectiveArea.
 */
void TurtleObjectiveState::onCollide(ObjectiveArea &other) {
    // Do nothing, already in objectiveState.
}

/**
 * Method for acting upon a collision with an obstacle.
 */
void TurtleObjectiveState::onCollideObstacle() {
    // Do nothing, turtle is immune.
}

/**
 * Move method for the turtle objective state.
 * This move method allows the player to move out of the screen in a swimming motion towards a set point.
 * @param inputData - Input data to move the turtle with.
 */
void TurtleObjectiveState::move(InputData *inputData) {
    // Check whether the turtle is within the screen.
    bool withinX = (turtle->getX() >= (-Variables::DESPAWN_MARGIN)
                    && turtle->getX() <= (Variables::PIXEL_WIDTH + Variables::DESPAWN_MARGIN));
    bool withinY = (turtle->getY() >= (-Variables::DESPAWN_MARGIN)
                    && turtle->getY() <= (Variables::PIXEL_HEIGHT + Variables::DESPAWN_MARGIN));

    if (withinX && withinY) {
        // If the turtle is still in the screen then move the turtle.
        double x = turtle->getX() + (turtle->getSpeed() * inputData->getFrameTime() * cos(turtle->getDirection()));
        double y = turtle->getY() + (turtle->getSpeed() * inputData->getFrameTime() * sin(turtle->getDirection()));
        turtle->setPosition(x, y);
    } else {
        // Else set the outOfScreen boolean, this will cause the TurtleManager to remove this turtle.
        turtle->setOutOfScreen();
    }

}

/**
 * Method that maintains counters to determine the rotation of the turtle.
 * After updating the counters, methods are called to act upon those counters and to rotate the turtle.
 * @param inputData - Input data to update the turtle counters with.
 */
void TurtleObjectiveState::updateCounters(InputData *inputData) {
    // Decrement the timer.
    turtle->setWiggleTimer(turtle->getWiggleTimer() - inputData->getFrameTime());

    // If the wiggleTimer ends we turn the turtle by changing the direction slightly in the opposite direction.
    // This causes the turtle to wiggle.
    if (turtle->getWiggleTimer() <= 0) {
        turn();
    }

    // Change the direction based on the linear interpolation between the start and goal direction.
    // This makes rotation smooth instead of discrete.
    double wiggleRotation =
            MathUtilities::linearInterpolation(Variables::WIGGLE_DURATION_OBJECTIVE - turtle->getWiggleTimer(),
                                               Variables::WIGGLE_DURATION_OBJECTIVE,
                                               startRotation, goalRotation);
    turtle->setDirection(wiggleRotation);
}

/**
 * Turn the turtle by changing its direction in the opposite direction.
 * This causes the turtle to wiggle.
 * startRotation, goalRotation and the wiggleTimer are set accordingly to allow a smooth rotation.
 */
void TurtleObjectiveState::turn() {
    // Set the WiggleTimer to the WIGGLE_DURATION, used to determine the end of the next wiggle.
    turtle->setWiggleTimer(Variables::WIGGLE_DURATION_OBJECTIVE);

    // Set the goalRotation to an offset to the opposite direction to make the turtle wiggle.
    turtle->setWiggleFactor(turtle->getWiggleFactor() * -1);
    this->startRotation = turtle->getDirection();
    this->goalRotation = startRotation + Variables::WIGGLE_OFFSET_OBJECTIVE * turtle->getWiggleFactor();
}

/**
 * Calculate the alpha of the turtle, dependent of the distance to the center of the sea.
 * The alpha value is a value between 0 and 255 that decides the opacity of a texture.
 * @return alpha - The alpha of the turtle.
 */
double TurtleObjectiveState::calculateAlpha() {
    double distance = MathUtilities::distanceBetweenPoints(objectiveArea.getSwimDestination().x,
                                                           objectiveArea.getSwimDestination().y, turtle->getX(),
                                                           turtle->getY());

    auto maxRange = objectiveArea.getRadius() * 2.0;
    double newAlpha = (distance / maxRange) * 255.0;

    // If the alpha tends to go up (which it shouldn't), return the old alpha value.
    if (newAlpha < turtle->getAlpha()) {
        turtle->setAlpha(newAlpha);
        return newAlpha;
    } else {
        return turtle->getAlpha();
    }
}

/**
 * Getter for the StateIdentifier of this state, this can be used to easily identify which instantiation of BasicState
 * this is. Mainly used for debug draws.
 * @return stateIdentifier - The string that represents the name of this state.
 */
std::string TurtleObjectiveState::getStateIdentifier() {
    stringstream stream;
    stream << this->turtle->getId() << ": TurtleObjectiveState";
    return stream.str();
}