//
// Created by maxim on 12-6-18.
//

#ifndef GAME_OBJECTIVEAREA_H
#define GAME_OBJECTIVEAREA_H

#include "turtle/Turtle.h"

/**
 * The objective area, the area the turtles need to reach.
 * The class provides a destination that turtles should swim to to ensure that
 * they move out of the screen.
 */
class ObjectiveArea {

private:
    double x;
    double y;
    double radius;
    double swimDirection;
    HitBox hitBox = HitBox(0, 0, 0);
    sf::Vector2f swimDestination;

    void calculateSwimDestination();

public:
    ObjectiveArea(double x, double y, double radius, double swimDirection);

    void render(RenderWrapper *renderWrapper);

    double getX() const;

    double getY() const;

    double getRadius() const;

    double getSwimDirection() const;

    const HitBox &getHitBox() const;

    const Vector2f &getSwimDestination() const;
};


#endif //GAME_OBJECTIVEAREA_H
