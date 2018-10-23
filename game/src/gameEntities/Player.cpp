//
// Created by thijmen on 14-5-18.
//

#include "Player.h"
#include "../storage/Variables.h"
#include "../rendering/RenderWrapper.h"
#include "../storage/InputData.h"
#include <iostream>
#include <cmath>

using namespace std;

/**
 * Constructor for the player object.
 * @param id - Unique id for the player.
 */
Player::Player(int id) {
    this->id = id;
    this->x = 100.0;
    this->y = 100.0;
    this->direction = 0.0;
    this->forceRange = Variables::scaleToPixels(Variables::PLAYER_FORCE_RADIUS);
    this->radius = Variables::scaleToPixels(Variables::PLAYER_WALL_RADIUS);
    this->active = true;
    this->forceColliding = false;
    this->wallColliding = false;
    this->trail = new Trail();
    this->timeOutTimer = Variables::PLAYER_TIMEOUT;
}

/**
 * Updates the player every tick.
 * @param inputX - New x position for the player.
 * @param inputY - New y position for the player.
 * @param inputType - Input type of the player.
 */
void Player::update(double inputX, double inputY, int inputType) {

    // If the input type is with the mouse, it is handled differently.
    if (inputType == InputData::mouse) {
        setDirection((atan2(inputY - getY(), inputX - getX()) * 180.0) / M_PI);
        setX(inputX);
        setY(inputY);
        trail->update(inputX, inputY, static_cast<int>(direction));
        // Else we have keyboard input.
    } else if (inputType == InputData::wasdkeys || inputType == InputData::arrowkeys) {
        setDirection((atan2(inputY - getY(), inputX - getX()) * 180.0) / M_PI);
        if ((getX() > 0 and inputX < 0) or (getX() < Variables::PIXEL_WIDTH and inputX > 0)) {
            setX(inputX + getX());
        }
        if ((getY() > 0 and inputY < 0) or (getY() < Variables::PIXEL_HEIGHT and inputY > 0)) {
            setY(inputY + getY());
        }
        trail->update(inputX + getX(), inputY + getY(), static_cast<int>((atan2(inputY, inputX) * 180.0) / M_PI));
    } else {
        setDirection((atan2(inputY - getY(), inputX - getX()) * 180.0) / M_PI);
        setX(inputX);
        setY(inputY);
        trail->update(inputX, inputY, static_cast<int>(direction));
    }
}

/**
 * Updates the trail.
 */
void Player::updateTrail(float time) {
    trail->updatePoints(time);
}

// LCOV_EXCL_START
/**
 * Renders the player.
 * @param renderWrapper - Wrapper for the render window.
 */
void Player::render(RenderWrapper *renderWrapper) {
    (*renderWrapper).renderPlayer(getX(), getY(), isForceColliding(), timeOutTimer);
}
// LCOV_EXCL_STOP

// Getters

/**
 * Getter for the direction the player is heading in.
 * @return direction - The direction the player is facing.
 */
double Player::getDirection() {
    return this->direction;
}

/**
 * Getter for the active status of the player.
 * This indicates whether the player is currently actively detected in the game by IMOVE.
 * @return active - If player is active or not.
 */
bool Player::isActive() {
    return this->active;
}

/**
 * Getter for the collision status of the player force area.
 * @return forceColliding - If the player force area is currently colliding with another object.
 */
bool Player::isForceColliding() {
    return this->forceColliding;
}

/**
 * Getter for the collision status of the player impenetrable wall.
 * @return wallColliding - If the player impenetrable wall is currently colliding with another object.
 */
bool Player::isWallColliding() {
    return this->wallColliding;
}

/**
 * Getter for the outer hitbox of the player.
 * This is the hitbox of the force range.
 * @return hitBox - The hitbox of the player.
 */
HitBox Player::getOuterHitBox() {
    HitBox hitBox(this->forceRange, getX(), getY());
    return hitBox;
}

/**
 * Getter for the player trail.
 * @return trail - The player trail.
 */
Trail *Player::getTrail() {
    return trail;
}

/**
 * Getter for the inputTimeOutTimer.
 * The PlayerManager uses this to remove players that have been added by IMOVE but that are stationary and not
 * coupled to an actual person.
 * This is caused by a incorrect tracking in IMOVE, thus after a timer they are automatically removed from the
 * game logic by the PlayerManager.
 * @return timeOutTimer - The current value of the timeOutTimer, if smaller or equal to zero, the player is bugged.
 */
double Player::getTimeOutTimer() const {
    return this->timeOutTimer;
}

// Setters

/**
 * Sets the x position of the player, resets the inputTimeOut timer.
 * @param x - The horizontal position to be set to.
 */
void Player::setX(double x) {
    if ((int) this->x != (int) x) {
        this->timeOutTimer = Variables::PLAYER_TIMEOUT;
    }
    this->x = x;
}

/**
 * Sets the y position of the player, resets the inputTimeOut timer.
 * @param y - The vertical position to be set to.
 */
void Player::setY(double y) {
    if ((int) this->y != (int) y) {
        this->timeOutTimer = Variables::PLAYER_TIMEOUT;
    }
    this->y = y;
}

/**
 * Sets the direction of the player.
 * @param direction - The direction to be set to.
 */
void Player::setDirection(double direction) {
    this->direction = direction;
}

/**
 * Sets the active status of the player.
 * @param status - The active status to be set to.
 */
void Player::setActive(bool status) {
    this->active = status;
}

/**
 * Sets the colliding status of the player force area.
 * This is the area the turtles are allowed in, and will move naturally.
 * @param forceCollide - The colliding status of the force area to be set to.
 */
void Player::setForceColliding(bool forceCollide) {
    this->forceColliding = forceCollide;
}

/**
 * Sets the colliding status of the player impenetrable wall.
 * This is the area that the turtles are not allowed in, and will be forcefully pushed out.
 * @param wallCollide - The colliding status of the wall to be set to.
 */
void Player::setWallColliding(bool wallCollide) {
    this->wallColliding = wallCollide;
}

/**
 * Setter for the timeOutTimer.
 * @param timeOutTimer - The new value of the timeOutTimer.
 */
void Player::setTimeOutTimer(double timeOutTimer) {
    Player::timeOutTimer = timeOutTimer;
}