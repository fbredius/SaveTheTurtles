//
// Created by maxim on 12-6-18.
//

#include <math.h>
#include "ObjectiveArea.h"
#include "../storage/Variables.h"

/**
 * Constructor for an ObjectiveArea.
 * Turtles have to be brought to an ObjectiveArea in order to score them.
 *
 * @param x - The x coordinate of the origin of the ObjectiveArea.
 * @param y - The y coordinate of the origin of the ObjectiveArea.
 * @param radius - The radius of the ObjectiveArea circle.
 * @param swimDirection - The direction in which turtles should swim upon entering the area. (in degrees)
 */
ObjectiveArea::ObjectiveArea(double x, double y, double radius, double swimDirection) {
    this->x = x;
    this->y = y;
    this->radius = radius;
    this->swimDirection = swimDirection * (M_PI / 180.0);
    this->hitBox = HitBox(this->radius, this->x, this->y);

    calculateSwimDestination();
}

/**
 * Method for calculating a point along the swimDirection that the Turtles should move to.
 */
void ObjectiveArea::calculateSwimDestination() {
    sf::Vector2f directionVector = sf::Vector2f(static_cast<float>(cos(swimDirection) * (Variables::VISIBILITY_MARGIN)),
                                                static_cast<float>(sin(swimDirection) *
                                                                   (Variables::VISIBILITY_MARGIN)));

    sf::Vector2f center = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));

    swimDestination = center + directionVector;
}

// LCOV_EXCL_START
/**
 * Renders the ObjectiveArea.
 * @param renderWrapper - The RenderWrapper that should be used for rendering.
 */
void ObjectiveArea::render(RenderWrapper *renderWrapper) {
    renderWrapper->renderObjectiveArea(x, y, radius, swimDirection);
}
// LCOV_EXCL_STOP

/**
 * Getter for the x coordinate of the ObjectiveArea.
 * @return x - The x coordinate.
 */
double ObjectiveArea::getX() const {
    return x;
}

/**
 * Getter for the y coordinate of the ObjectiveArea.
 * @return y - The y coordinate.
 */
double ObjectiveArea::getY() const {
    return y;
}

/**
 * Getter for the radius coordinate of the ObjectiveArea.
 * @return radius - The radius of the circle.
 */
double ObjectiveArea::getRadius() const {
    return radius;
}

/**
 * Getter for the swimDirection of the ObjectiveArea.
 * @return swimDirection - The direction that the Turtles that enter the ObjectiveArea should swim to.
 */
double ObjectiveArea::getSwimDirection() const {
    return swimDirection;
}

/**
 * Getter for the HitBox, used for collision detection.
 * @return hitBox - The circle hitBox of the ObjectiveArea.
 */
const HitBox &ObjectiveArea::getHitBox() const {
    return hitBox;
}

/**
 * Getter for the swimDestination that Turtles in the TurtleObjectiveState swim towards.
 * @return swimDestination - A vector containing the destination coordinates.
 */
const Vector2f &ObjectiveArea::getSwimDestination() const {
    return swimDestination;
}