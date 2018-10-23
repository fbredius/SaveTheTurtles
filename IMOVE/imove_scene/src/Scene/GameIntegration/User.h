//
// Created by raoul on 23-5-18.
//

#ifndef IMOVE_USER_H
#define IMOVE_USER_H

#include <list>
#include "../../../../util/src/Vector2.h"

/**
 * Every User in the scene has a location and an ID. These IDs match the IDs of User in InputData / PlayerManager.
 */
class User {
private:
    int id;
    float x;
    float y;
    std::list<float> avgX;
    std::list<float> avgY;
    int bufferSize = 5;

public:
    User(int id, float x, float y);

    // Getters
    int getId() const;

    float getX();

    float getY();

    // Setters
    void setLocation(float x, float y);
};

#endif //IMOVE_USER_H
