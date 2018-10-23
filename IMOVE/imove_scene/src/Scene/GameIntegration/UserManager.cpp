#ifndef IMOVE_USERMANAGER_CPP
#define IMOVE_USERMANAGER_CPP


#include <iostream>
#include "../../../../util/src/Vector2.h"
#include "User.h"
#include "UserManager.h"
#include <list>

/**
 * Constructor for the UserManager.
 * As only the instance needs to be created, the constructor is empty.
 */
UserManager::UserManager() : data() {}

/**
 * Add a new user to the instance.
 * @param user - The user to be added to the UserManager.
 */
void UserManager::add(User *user) {
    active.push_back(user);
}

/**
 * Schedule a user to be deleted.
 * @param id - The ID of the user that has to be removed.
 */
void UserManager::scheduleForRemoval(int id) {
    this->scheduleForRemoval(this->get(id));
}

/**
 * Schedule a user to be deleted
 * @param user - The instance of user that has to be removed.
 */
void UserManager::scheduleForRemoval(User *user) {
    scheduledForRemoval.push_back(user);
}

/**
 * This method removes all the user from the scene that are stored in the scheduledForRemoval list.
 * It looks at every active user, and determines if it is in scheduledForRemoval.
 * If not, it is added to the new list, after which that list is assigned to the current active users.
 * The scheduledForRemoval list is then flushed.
 */
void UserManager::removeAll() {
    std::list<User *> newActive;
    for (User *userActive : active) {
        bool remove = false;
        for (User *userToRemove: scheduledForRemoval) {
            remove = userActive->getId() == userToRemove->getId();
            if (remove) { break; }
        }
        if (!remove) {
            newActive.push_back(userActive);
        }
    }

    active.clear();
    active = newActive;
    scheduledForRemoval.clear();
}

/**
 * Getter for the size
 * @return size - The size of the active users in the UserManager.
 */
int UserManager::size() {
    return (int) active.size();
}

/**
 * Getter for all the active players
 * @return active - All active players in UserManager
 */
std::list<User *> UserManager::getAllUsers() {
    return active;
}

/**
 * Checks if a user's id is among the active users's ids.
 * @param id - The ID of the user that you want to check for presence.
 * @return bool - Return true if user's ID is found, false otherwise.
 */
bool UserManager::has(int id) {
    bool has = false;
    for (auto user : active) {
        has = user->getId() == id;
        if (has) {
            break;
        }
    }
    return has;
}

/**
 * Checks if a user is among the active users.
 * @param user - The user that you want to check for presence.
 * @return bool - Return true if user is found, false otherwise.
 */
bool UserManager::has(User *user) {
    return this->has(user->getId());
}

/**
 * Getter for a user referenced by id.
 * @param id - The ID of the user that needs to be returned.
 * @return user - Returns the user, or an invalid user. You are expected to check for validity by asserting that (UserManager.get(x).getId() != -1).
 */
User *UserManager::get(int id) {
    for (User *user : active) {
        if (user->getId() == id) {
            return user;
        }
    }
    return new User{-1, -1, -1};
}

/**
 * Get all the active players.
 * @return active - All active players in the UserManager.
 */
std::list<User *> UserManager::getAllPlayers() {
    return active;
}

/**
 * Method for debugging purposes, which returns if the scheduledForRemoval is empty.
 * This can for example be used in tests, as this needs to be true after every call of GameScene.processingPeople().
 * @return bool - Returns true if scheduledForRemoval is empty, false otherwise.
 */
bool UserManager::isRemoved() {
    return scheduledForRemoval.empty();
}

/**
 * Clear all the data that is generated for the movement of all the players in the scene.
 */
void UserManager::clearData() {
    for (InputData *d : data) {
        delete d;
    }
    data.clear();
}

/**
 * Clears all users that are active in the scene at the moment.
 */
void UserManager::clearUsers() {
    for (User *u : active) {
        delete u;
    }
    active.clear();
}

/**
 * Generates InputData based on the current active users.
 * @return data - A list with all InputData of all users, which is propagated to the GameManager.
 */
std::list<InputData *> UserManager::generateInput(float frameTime) {
    for (User *user : active) {
        auto *input = new InputData(InputData::imove);
        input->setId(user->getId());
        input->setPlayerX(user->getX());
        input->setPlayerY(user->getY());
        input->setFrameTime(frameTime);
        input->setValid();
        data.push_back(input);
    }
    return data;
}


#endif //IMOVE_USERMANAGER_CPP
