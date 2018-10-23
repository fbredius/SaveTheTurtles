//
// Created by raoul on 5-6-18.
//

#include <iostream>
#include "Trail.h"
#include "../storage/Variables.h"

/**
 * Constructor for the Trail Points. It initializes the timer.
 */
Trail::Trail() {
    resetTimer();
}

// LCOV_EXCL_START
/**
 * Renders the trail.
 * @param renderWrapper - Wrapper for the render window.
 */
void Trail::render(RenderWrapper *renderWrapper) {
    renderWrapper->renderTrail(getTrailPoints());

}
// LCOV_EXCL_STOP

//Getters

/**
 * Getter for the current time of the trail. The timer resets every iteration.
 * @return timer - The current time that the timer is on.
 */
double Trail::getTime() {
    return timer;
}

/**
 * Getter for all the points maintained in the trail.
 * @return points - All TrailPoints within this Trail.
 */
std::list<TrailPoint *> Trail::getTrailPoints() {
    return points;
}

// Setters

/**
 * Resets the timer for when the next coordinates needs to be added.
 */
void Trail::resetTimer() {
    this->timer = 1 / Variables::TRAIL_FREQUENCY;
}

/**
 * This is called every frame, such that fading can be done, even when the Player is not moving.
 * Furthermore, when a point has faded (e.g. opacity <= 0), then it can be scheduled for removal.
 * @param time - The frame time that needs to be subtracted from the timer.
 */
void Trail::updatePoints(float time) {
    timer -= time;
    for (TrailPoint *point : points) {
        if (point->getOpacity() <= 0) {
            toBeRemoved.push_back(point);
        }
        point->decreaseFadeTimer(time);
    }
    removePoints();
}

/**
 * This method subtracts all points in toBeRemoved from points. This entails in all faded points being removed.
 */
void Trail::removePoints() {
    std::list<TrailPoint *> newActive;
    for (TrailPoint *point : points) {
        bool remove = false;
        for (TrailPoint *removePoint: toBeRemoved) {
            remove = point->getX() == removePoint->getX() && point->getY() == removePoint->getY() &&
                     point->getDirection() == removePoint->getDirection();
            if (remove) { break; }
        }
        if (!remove) {
            newActive.push_back(point);
        }
    }

    points.clear();
    points = newActive;
    toBeRemoved.clear();
}

/**
 * Is called every time the player moves. If the timer indicates that a new point should be drawn, add the location as a
 * trailpoint and reset the timer.
 * @param x - The x location of the player.
 * @param y - The y location of the player.
 * @param direction - The angle of the player.
 */
void Trail::update(double x, double y, int direction) {
    if (timer <= 0) {
        points.push_back(new TrailPoint(x, y, direction));
        resetTimer();
    }
}