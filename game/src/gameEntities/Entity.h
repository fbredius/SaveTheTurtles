//
// Created by timbuntu on 7-6-18.
//

#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "../storage/HitBox.h"

/**
 * Game entity class.
 */
class Entity {

protected:
    int id;
    double x;
    double y;
    double radius;

public:

    // Getters
    /**
     * Getter for the x value of the entity.
     * @return x - The x coordinate.
     */
    double getX() {
        return this->x;
    }

    /**
     * Getter for the y value of the entity.
     * @return y - The y coordinate.
     */
    double getY() {
        return this->y;
    }

    /**
     * Getter for the id of the entity.
     * @return id - The id.
     */
    const int getId() const {
        return this->id;
    }

    /**
     * Getter for the radius of the entity.
     * @return radius - The radius.
     */
    double getRadius() {
        return this->radius;
    }

    /**
     * Getter for the hit-box.
     * @return A hit-box created according to the entity's position and radius.
     */
    HitBox getHitBox() {
        return HitBox(this->radius, this->x, this->y);
    }
};

#endif //GAME_ENTITY_H
