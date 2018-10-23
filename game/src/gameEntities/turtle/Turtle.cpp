//
// Created by timbuntu on 8-5-18.
//

#include "Turtle.h"
#include "../../utility/BasicState.h"
#include "../../storage/Variables.h"
#include "turtleStates/TurtleSpawnState.h"
#include "turtleStates/TurtleReturningState.h"
#include "turtleStates/TurtleWalkState.h"
#include "../../utility/MathUtilities.h"
#include "turtleStates/TurtleIdleState.h"
#include "turtleStates/TurtleObjectiveState.h"
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iomanip>

using namespace sf;

/**
 * Constructor for the turtle object.
 * @param id - Unique turtle id for this turtle.
 */
Turtle::Turtle() {
    this->currentState = new TurtleSpawnState(this);
    this->id = Variables::getNewId();
    init();
}

/**
 * Copy constructor to prevent memory leaks when creating another GameManager.
 * @param obj - existing Turtle that is to be copied.
 */
Turtle::Turtle(const Turtle &obj) {
    *currentState = *obj.getCurrentState();
    this->id = obj.getId();
    init();
}

/**
 * Initializes all the parameters of a new Turtle to default parameters.
 */
void Turtle::init() {
    this->alive = true;
    this->colliding = false;

    this->returning = false;
    this->idle = false;
    this->roaming = false;
    this->inObjective = false;
    this->outOfScreen = false;
    this->interactable = false;
    this->resetting = false;
    this->againstWall = false;

    this->radius = Variables::scaleToPixels(Variables::TURTLE_RADIUS);
    int randomValue = rand();
    this->direction = (randomValue / (2 * M_PI) - floor(randomValue / (2 * M_PI))) * 2 * M_PI;
    this->speed = Variables::scaleToPixels(Variables::TURTLE_WALK_SPEED);

    this->wiggling = false;
    this->wiggleTimer = 0.0;
    this->wiggleFactor = 1.0;
    this->moveTimer = 0.0;

    // The opacity of the turtle sprite. (255 = opaque, 0 = transparent)
    this->alpha = 255.0;

    spawn();
}

/**
 * Destructor for de-allocating memory.
 */
Turtle::~Turtle() {
    delete currentState;
}

// GAME-LOGIC VOID METHODS

/**
 * Updates the turtle every tick.
 * Checks if the turtle should be moving.
 * @param inputList - The list of input types.
 */
void Turtle::update(std::list<InputData *> inputList) {
    currentState->update(inputList);
}

// LCOV_EXCL_START
/**
 * Renders the turtle.
 * @param renderWrapper - Wrapper for the render window.
 */
void Turtle::render(RenderWrapper *renderWrapper) {
    if (alive || inObjective) {
        currentState->render(renderWrapper);
    }
}
// LCOV_EXCL_STOP

/**
 * Sets the Turtle's living status to dead and updates the amount of dead turtles.
 */
void Turtle::kill() {
    Variables::DEAD_TURTLES++;
    this->alive = false;
}

/**
 * Sets the Turtles's location to a random location using MathUtilities.
 */
void Turtle::spawn() {
    pair<double, double> location = MathUtilities::randomSpawnLocation();
    this->x = location.first;
    this->y = location.second;
}

/**
 * Hatches the egg and spawns the turtle.
 * The turtles enters the walk state and starts walking around.
 */
void Turtle::hatch() {
    BasicState *oldState = this->getCurrentState();
    this->setCurrentState(new TurtleIdleState(this));
    delete oldState;
    this->interactable = true;
}

/**
 * Calculates the direction to move to a certain point.
 * @param returnX - The X coordinate of the goal.
 * @param returnY - The Y coordinate of the goal.
 * @return direction - The calculated direction towards the goal coordinates.
 */
double Turtle::calculateDirection(double returnX, double returnY) {
    double x = returnX - getX();
    double y = returnY - getY();

    double direction;
    if (x == 0 and y > 0) {
        direction = 1.5 * M_PI;
    } else if (x == 0 and y < 0) {
        direction = 0.5 * M_PI;
    } else if (x == 0 and y == 0) {
        return this->direction;
    } else {
        direction = atan2(y, x);
    }
    return direction;
}

//GETTERS

/**
 * Getter for the current turtle state.
 * @return the current state of the turtle.
 */
BasicState *Turtle::getCurrentState() const {
    return currentState;
}

/**
 * Getter for the living status of the turtle.
 * @return alive - Alive status of this turtle.
 */
bool Turtle::isAlive() const {
    return alive;
}

/**
 * Getter for the idle state of the turtle.
 * @return idle - If the turtle is idle.
 */
bool Turtle::isIdle() const {
    return idle;
}

/**
 * Getter for the roaming state of the turtle.
 * @return roaming - If the turtle is roaming.
 */
bool Turtle::isRoaming() const {
    return roaming;
}

/**
 * Getter for the direction the turtle is heading in.
 * @return direction - The direction this turtle is facing.
 */
double Turtle::getDirection() const {
    return direction;
}

/**
 * Getter for the returning state of the turtle.
 * @return returning - If the turtle is returning to the spawn.
 */
bool Turtle::isReturning() const {
    return returning;
}

/**
 * Getter for the speed of the turtle.
 * @return speed - Speed of this turtle.
 */
double Turtle::getSpeed() const {
    return speed;
}

/**
 * Getter for wiggling.
 * @return wiggling - The wiggling status of this turtle.
 */
bool Turtle::isWiggling() const {
    return wiggling;
}

/**
 * Getter for wiggleTime.
 * @return wiggleTimer - The time that the turtle will keep wiggling for.
 */
double Turtle::getWiggleTimer() const {
    return wiggleTimer;
}

/**
 * Getter for wiggleFactor.
 * @return wiggleFactor - The wiggleFactor of this turtle.
 */
double Turtle::getWiggleFactor() const {
    return wiggleFactor;
}

/**
 * The turtle's timer to changing direction.
 * @return moveTimer - The time until the turtle moves again.
 */
double Turtle::getMoveTimer() const {
    return moveTimer;
}

/**
 * Getter to check if the turtle is colliding.
 * @return colliding - Colliding status of this turtle.
 */
bool Turtle::isColliding() const {
    return colliding;
}

/**
 * Get the turtles out of screen status.
 * @return outOfScreen - Out of screen status.
 */
bool Turtle::isOutOfScreen() const {
    return outOfScreen;
}

/**
 * Get the turtle interactable ness.
 * @return interactable - If the turtle is interactable.
 */
bool Turtle::isInteractable() const {
    return interactable;
}

/**
 * Get whether the turtle is resetting or not, this is set to true by the GameResetState.
 * @return resetting - True if the turtle is resetting and thus walking back to the center.
 */
bool Turtle::isResetting() const {
    return resetting;
}

/**
 * Get the turtle alpha.
 * The opacity of the turtle sprite. (255 = opaque, 0 = transparent)
 * @return alpha - The turtle alpha.
 */
double Turtle::getAlpha() const {
    return alpha;
}

/**
 * Getter for whether the turtle is against the borders at the edge of the screen, it should be slower in this case.
 * @return againstWall - a boolean determining whether the turtle is against the wall.
 */
bool Turtle::isAgainstWall() const {
    return againstWall;
}

//SETTERS

/**
 * Set the current turtle state, used when swapping states based on turtle location or player interaction.
 * @param currentState - the new turtle state.
 */
void Turtle::setCurrentState(BasicState *currentState) {
    Turtle::currentState = currentState;
}

/**
 * Setter for id.
 * @param id - Unique id of this turtle.
 */
void Turtle::setId(int id) {
    this->id = id;
}

/**
 * Setter for the alive boolean.
 * @param alive - Whether the turtle is alive or not.
 */
void Turtle::setAlive(bool alive) {
    this->alive = alive;
}

/**
 * Sets the returning state of the turtle.
 * @param returning - If the turtle is returning.
 */
void Turtle::setReturning(bool returning) {
    this->returning = returning;
}

/**
 * Sets the idle state of the turtle.
 * @param idle - If the turtle is idle.
 */
void Turtle::setIdle(bool idle) {
    this->idle = idle;
}

/**
 * Sets the roaming state of the turtle.
 * @param roaming - If the turtle is roaming.
 */
void Turtle::setRoaming(bool roaming) {
    this->roaming = roaming;
}

/**
 * Sets the x position of the turtle.
 * If the turtle tends to go out of the screen on the x axis, run back to the spawn.
 * @param x - The horizontal position to be set to.
 */
void Turtle::setX(double x) {
    if ((x >= 0 and x <= Variables::PIXEL_WIDTH) or inObjective) {
        this->x = x;
    } else {
        setCurrentState(new TurtleReturningState(this));
    }

}

/**
 * Sets the y position of the turtle.
 * If the turtle tends to go out of the screen on the y axis, run back to the spawn.
 * @param y - The vertical position to be set to.
 */
void Turtle::setY(double y) {
    if ((y >= 0 and y <= Variables::PIXEL_HEIGHT) or inObjective) {
        this->y = y;
    } else {
        setCurrentState(new TurtleReturningState(this));
    }

}

/**
 * Method that sets the position of a turtle while checking if its against the wall.
 * This should be used for movement from the TurtleStates to prevent them from walking through the border.
 * @param x - The x position that the turtle should be set to.
 * @param y - The y position that the turtle should be set to.
 */
void Turtle::setPosition(double x, double y) {

    // If the turtle is in the objective, do not do wall checks.
    if (inObjective) {
        this->x = x;
        this->y = y;
        this->againstWall = false;
        return;
    }

    // Initialize values.
    double offset = Variables::scaleToPixels(Variables::TURTLE_BORDER_OFFSET);
    bool againstWall = false;

    // Cap movement on the x-axis.
    if (x < offset) {
        againstWall = true;
        x = offset;
    } else if (x > Variables::PIXEL_WIDTH - offset) {
        againstWall = true;
        x = Variables::PIXEL_WIDTH - offset;
    }

    // Cap movement on the y-axis.
    if (y < offset) {
        againstWall = true;
        y = offset;
    } else if (y > Variables::PIXEL_HEIGHT - offset) {
        againstWall = true;
        y = Variables::PIXEL_HEIGHT - offset;
    }

    // Set the results.
    this->againstWall = againstWall;
    this->x = x;
    this->y = y;
}

/**
 * Sets the direction of the turtle.
 * @param direction - The direction to be set to.
 */
void Turtle::setDirection(double direction) {
    this->direction = direction;
}

/**
 * Sets the speed of the turtle.
 * @param speed - The speed to be set to.
 */
void Turtle::setSpeed(double speed) {
    this->speed = speed;
}

/**
 * Sets if the turtle is in the objective area.
 * @param inObjective - If the turtle is in the objective area.
 */
void Turtle::setInObjective(bool inObjective) {
    this->inObjective = inObjective;
    this->inObjective = inObjective;
}

/**
 * Colors the turtle so it is visible that there is collision.
 * @param collide - Boolean if the turtle is currently colliding.
 */
void Turtle::setColliding(bool collide) {
    colliding = collide;
    setWiggling(collide);
}

/**
 * Setter for timer.
 * @param timer - The time for the turtle to be changing directions again.
 */
void Turtle::setMoveTimer(double timer) {
    this->moveTimer = timer;
}

/**
 * Sets the wiggling of the turtle.
 * @param wiggling - Boolean if the turtle should be wiggling.
 */
void Turtle::setWiggling(bool wiggling) {
    this->wiggling = wiggling;
}

/**
 * Setter for wiggle time.
 * @param wiggleTime - The time for the turtle to be wiggling again.
 */
void Turtle::setWiggleTimer(double wiggleTime) {
    this->wiggleTimer = wiggleTime;
}

/**
 * Setter for the wiggle factor.
 * @param wiggleFactor - The factor for the turtle wiggling.
 */
void Turtle::setWiggleFactor(double wiggleFactor) {
    Turtle::wiggleFactor = wiggleFactor;
}

/**
 * Set the boolean to true for the turtle being out of the screen.
 */
void Turtle::setOutOfScreen() {
    this->outOfScreen = true;
}

/**
 * If an egg is spawning, the player cannot interact with it, once its spawned, this function is called.
 */
void Turtle::setInteractable() {
    this->interactable = true;
}

/**
 * Sets whether the turtle is resetting according to the natural reset mechanic.
 * The turtle returns but can be interrupted.
 * @param value - Whether the turtle is resetting.
 */
void Turtle::setResetting(bool value) {
    this->resetting = value;
}

/**
 * Set the alpha of the current turtle sprite.
 * The opacity of the turtle sprite. (255 = opaque, 0 = transparent)
 * @param alpha - The new alpha value.
 */
void Turtle::setAlpha(double alpha) {
    this->alpha = alpha;
}