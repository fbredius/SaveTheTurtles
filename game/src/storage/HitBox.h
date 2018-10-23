//
// Created by thijmen on 22-5-18.
//

#ifndef GAME_HITBOX_H
#define GAME_HITBOX_H

/**
 * The hitBox object, the object that is used to describe a circular hitBox.
 * The CollisionHandler uses instances of this class to register collisions.
 */
class HitBox {

private:
    double radius;
    double x;
    double y;

public:
    HitBox(double radius, double x, double y);

    // Getters
    double getRadius();

    double getX();

    double getY();

    // Setters
    void setRadius(double radius);

    void setX(double x);

    void setY(double y);
};

#endif //GAME_HITBOX_H
