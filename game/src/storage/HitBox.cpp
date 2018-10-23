//
// Created by thijmen on 22-5-18.
//

#include "HitBox.h"

/**
 * Constructor of the circular hitbox object.
 * @param radius - The radius of the hitbox.
 * @param x - The x location of the hitbox.
 * @param y - The y location of the hitbox.
 */
HitBox::HitBox(double radius, double x, double y) {
    this->radius = radius;
    this->x = x;
    this->y = y;
}

// Getters

/**
 * Getter of the hitbox radius.
 * @return radius - The hitbox radius.
 */
double HitBox::getRadius() {
    return radius;
}

/**
 * Getter of the hitbox x location.
 * @return x - The hitbox x location.
 */
double HitBox::getX() {
    return x;
}

/**
 * Getter of the hitbox y location.
 * @return y - The hitbox y location.
 */
double HitBox::getY() {
    return y;
}

// Setters

/**
 * Setter for the hitbox radius.
 * @param radius - The new hitbox radius.
 */
void HitBox::setRadius(double radius) {
    this->radius = radius;
}

/**
 * Setter for the hitbox x location.
 * @param x - The new hitbox x location.
 */
void HitBox::setX(double x) {
    this->x = x;
}

/**
 * Setter for the hitbox y location.
 * @param y - The new hitbox y location.
 */
void HitBox::setY(double y) {
    this->y = y;
}