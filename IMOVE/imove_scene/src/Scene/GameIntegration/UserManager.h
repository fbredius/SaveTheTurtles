#ifndef IMOVE_USERMANAGER_H
#define IMOVE_USERMANAGER_H

#include <list>
#include "User.h"
#include "../../../../../game/src/storage/InputData.h"

/**
 * All Users that are active in the scene are stored in this repository.
 * With this class, users can be added to or (scheduled to be) removed from the scene.
 */
class UserManager {
private:

    // Map to store Users based on id's of the corresponding person
    std::list<User *> active;

    // Vector containing id's of Users that need to be removed
    std::list<User *> scheduledForRemoval;

    std::list<InputData *> data;

public:
    UserManager();

    // Getters
    User *get(int id);

    std::list<User *> getAllPlayers();

    void clearData();

    void clearUsers();

    std::list<User *> getAllUsers();

    int size();

    void add(User *user);

    void scheduleForRemoval(int id);

    void scheduleForRemoval(User *user);

    void removeAll();

    bool has(int id);

    bool has(User *user);

    bool isRemoved();

    std::list<InputData *> generateInput(float frameTime);

};

#endif //IMOVE_USERMANAGER_H
