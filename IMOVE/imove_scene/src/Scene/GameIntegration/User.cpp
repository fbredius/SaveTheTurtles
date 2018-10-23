
/**
 * This class is used to store User.
 * Users are active within the GameScene and have a specified id and location.
 *
 * Created by raoul on 23-5-18.
 */

#ifndef IMOVE_USER_CPP
#define IMOVE_USER_CPP


#include <iostream>
#include "User.h"
#include "../../../../util/src/Vector2.h"
#include "../../../../../game/src/utility/MathUtilities.h"

/**
 * Constructor for user.
 * @param id - ID of the user.
 * @param x - Current x coordinate of the user.
 * @param y - Current y coordinate of the user.
 */
User::User(int id, float x, float y) {
    this->id = id;
    avgX.push_back(x);
    avgY.push_back(y);
    this->x = x;
    this->y = y;
}

// Getters

/**
 * Getter for the ID.
 * @return id - the ID of the user.
 */
int User::getId() const {
    return id;
}


/**
 * Getter for x coordinate.
 * @return x - the current x coordinate of the user.
 */
float User::getX() {
    std::list<float>::iterator it;
    float count = 0;
    for (it = avgX.begin(); it != avgX.end(); ++it) {
        count += (*it);
    }
    return count / (float) avgX.size();
}

/**
 * Getter for the y coordinate.
 * @return y - the current y coordinate of the user.
 */
float User::getY() {
    std::list<float>::iterator it;
    float count = 0;
    for (it = avgY.begin(); it != avgY.end(); ++it) {
        count += (*it);
    }

    return count / (float) avgY.size();
}

// Setters

/**
 * The setter for location.
 * @param x - The new x coordinate of the user.
 * @param y - The new y coordinate of the user.
 */
void User::setLocation(float x, float y) {
    if ((int) avgX.size() > bufferSize) {
        avgX.pop_front();
    }
    if ((int) avgY.size() > bufferSize) {
        avgY.pop_front();
    }
    avgX.push_back(x);
    avgY.push_back(y);
    this->x = x;
    this->y = y;
}

/**
 *
 * @param user1 - User instance that is compared.
 * @param user2 - User instance that user1 is compared to.
 * @return bool - Returns if the IDs of the users match.
 */
bool operator==(User user1, User user2) {
    return user1.getId() == user2.getId();
}


#endif //IMOVE_USER_CPP
