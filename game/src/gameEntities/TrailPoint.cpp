//
// Created by raoul on 5-6-18.
//

#include <iostream>
#include "TrailPoint.h"
#include "../storage/Variables.h"

/**
 * Constructor for the Trail Point. It sets the location angle, and initializes the opacity float.
 * @param x - The x location of the trail.
 * @param y - The y location of the trail.
 * @param direction - The direction the trail is moving in.
 */
TrailPoint::TrailPoint(double x, double y, int direction) {
    this->x = x;
    this->y = y;
    this->direction = direction;
    this->opacity = 1.0;
    this->fadeTimer = Variables::TRAIL_DURATION;
    this->textureId = rand() % 2 + 1;
}

//Getters

/**
 * The getter for the x coordinate of the TrailPoint.
 * @return x - The x coordinate of the TrailPoint.
 */
double TrailPoint::getX() {
    return x;
}

/**
 * The getter for the y coordinate of the TrailPoint.
 * @return y - The y coordinate of the TrailPoint.
 */
double TrailPoint::getY() {
    return y;
}

/**
 * The getter for the angle of the TrailPoint.
 * @return direction - The angle of the TrailPoint.
 */
int TrailPoint::getDirection() {
    return direction;
}

/**
 * The getter for the opacity of the TrailPoint.
 * @return opacity - The opacity of the TrailPoint.
 */
double TrailPoint::getOpacity() {
    return opacity;
}

/**
 * The getter for the texture id of the TrailPoint
 * @return textureId - The id of the texture of the TrailPoint
 */
int TrailPoint::getTextureId() {
    return textureId;
}

//Setters

/**
 * The setter for the x coordinate of the TrailPoint.
 * @param x - The x coordinate to be set.
 */
void TrailPoint::setX(int x) {
    this->x = x;
}

/**
 * The setter for the y coordinate of the TrailPoint.
 * @param x - The y coordinate to be set.
 */
void TrailPoint::setY(int y) {
    this->y = y;
}

/**
 * The setter for the direction of the TrailPoint
 * @param x - The direction to be set.
 */
void TrailPoint::setDirection(int direction) {
    this->direction = direction;
}

/**
 * This is called on every frame within the updatePoints function in Trail.
 * @param time - The frame time to be subtracted from the timer.
 */
void TrailPoint::decreaseFadeTimer(double time) {
    fadeTimer -= time;
    if (fadeTimer <= 0) {
        opacity -= Variables::TRAIL_FADE_SPEED;
    }
}