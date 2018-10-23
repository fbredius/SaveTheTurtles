//
// Created by timbuntu on 4-6-18.
//

#include "Obstacle.h"
#include "../storage/Variables.h"
#include <cstdlib>
#include <iostream>

using namespace std;

/**
 * Constructor for the obstacle class.
 * @param x - The x coordinate of the obstacle.
 * @param y - The y coordinate of the obstacle.
 */
Obstacle::Obstacle(double x, double y) {
    this->id = Variables::getNewId();
    this->x = x;
    this->y = y;
    this->alpha = 0;
    this->fade = false;
    // Modulo 60 - 30 reduces the rotation to the range [-30,30].
    this->rotation = (rand() % 60) - 30;
    this->radius = Variables::scaleToPixels(Variables::OBSTACLE_RADIUS);
}

/**
 * Update method for the obstacle.
 * This is used for fading the obstacle in and out based on frameTime.
 * @param inputList - InputData objects with a specified frameTime that is used for consistent fading.
 */
void Obstacle::update(std::list<InputData *> inputList) {
    // Return if there was no input because then we cannot determine how fast to fade.
    if (inputList.empty()) {
        return;
    }

    if (fade) {
        // Fade out the obstacle if it is marked for fading out.
        float frameTime = inputList.front()->getFrameTime();
        this->alpha -= frameTime * Variables::OBSTACLE_FADE_SPEED;
    } else if (alpha < 255.0) {
        // Fade in the obstacle when it just spawned.
        float frameTime = inputList.front()->getFrameTime();
        this->alpha += frameTime * Variables::OBSTACLE_FADE_SPEED;;
    }
}

// LCOV_EXCL_START
/**
 * Renders the obstacle in the window.
 * @param renderWrapper - Wrapper used for rendering.
 */
void Obstacle::render(RenderWrapper *renderWrapper) {
    renderWrapper->renderObstacle(this->getX(), this->getY(), this->getRotation(), alpha, this->getId());
}
// LCOV_EXCL_STOP

/**
 * Getter for the rotation.
 * @return rotation - The object rotation.
 */
double Obstacle::getRotation() {
    return this->rotation;
}

/**
 * Method for starting a fade, this initiates the removal process of an Obstacle when it collides with a Turtle.
 */
void Obstacle::startFade() {
    this->fade = true;
}

/**
 * Getter for the fading boolean, if a boolean is fading it cannot collide.
 * @return fade - True if the obstacle is fading.
 */
bool Obstacle::isFading() const {
    return fade;
}

/**
 * The current alpha value of the obstacle, used to determine if the obstacle is done fading.
 * @return alpha - The current alpha value that determines the transparency of the obstacle.
 */
double Obstacle::getAlpha() const {
    return alpha;
}

/**
 * The entity id of the obstacle.
 * @return id - The id of the obstacle.
 */
int Obstacle::getId() const {
    return id;
}