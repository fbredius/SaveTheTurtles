//
// Created by thijmen on 14-5-18.
//

#include "PlayerManager.h"
#include "../storage/InputData.h"
#include "../gameEntities/Player.h"
#include "../storage/Variables.h"
#include <iostream>
#include <list>

using namespace std;

/**
 * Constructor for the PlayerManager.
 */
PlayerManager::PlayerManager() {
    this->idCounter = -1;
}

/**
 * Updates the PlayerManager every tick.
 * This moves all players based on input and handles debug inputs.
 * @param inputList - The list of input types.
 */
void PlayerManager::update(std::list<InputData *> inputList) {

    if (inputList.empty()) {
        return;
    }

    // When the K key is pressed, all players are killed. This is for debugging purposes.
    if (inputList.front()->isKPressed()) {
        destroyAllPlayers();
        return;
    }

    // When the L key is pressed, a player is added. This is for debugging purposes.
    if (inputList.front()->isLPressed()) {
        createPlayer();
        return;
    }

    // Set the DEBUG variable for number of players.
    Variables::DEBUG_PLAYER_COUNT = numberOfPlayers();

    // Iterate over the different input data types.
    list<InputData *>::iterator inputIt;

    // Update all users with the single InputData object.
    // In the future, different users will have individual InputData objects.
    list<Player *>::iterator it;
    for (inputIt = inputList.begin(); inputIt != inputList.end(); ++inputIt) {
        // Do nothing if the input is not valid.
        // This is the case when there was no input detection during this frame for this player.
        if ((*inputIt)->isValid()) {
            for (it = players.begin(); it != players.end(); ++it) {
                if ((*inputIt)->getType() == 3) {
                    if ((*it)->getId() == (*inputIt)->getId()) {
                        (*it)->update((*inputIt)->getPlayerX(), (*inputIt)->getPlayerY(), (*inputIt)->getType());
                    }
                } else if ((*it)->getId() == (*inputIt)->getType()) {
                    (*it)->update((*inputIt)->getPlayerX(), (*inputIt)->getPlayerY(), (*inputIt)->getType());
                }
            }
        }
    }

    // Automatically destroy players that are decoupled from a person after their timeOut timer reaches zero.
    if (Variables::IMOVE_ACTIVE && Variables::AUTO_DESTROY_PLAYERS) {
        float frameTime = inputList.front()->getFrameTime();
        for (Player *player: players) {
            if (player->getTimeOutTimer() - frameTime <= 0) {
                this->destroyPlayer(player->getId());
                break;
            } else {
                player->setTimeOutTimer(player->getTimeOutTimer() - frameTime);
            }
        }
    }

    // Update the trail of the players, this only serves as a graphical particle.
    for (Player *player: players) {
        player->updateTrail((*inputList.front()).getFrameTime());
    }

}

// LCOV_EXCL_START
/**
 * Renders everything the PlayerManager manages: all the players.
 * @param renderWrapper - Wrapper for the render window.
 */
void PlayerManager::renderPlayers(RenderWrapper *renderWrapper) {
    list<Player *>::iterator it;
    for (it = players.begin(); it != players.end(); ++it) {
        (*it)->render(renderWrapper);
        (*it)->getTrail()->render(renderWrapper);
    }
}
// LCOV_EXCL_STOP

// SETTERS

/**
 * Creates a player with a new id and adds it to the players list.
 */
void PlayerManager::createPlayer() {
    int t = nextId();
    players.push_back(new Player(t));
}

/**
 * Creates a player with a specific id and adds it to the players list.
 * This is used by IMOVE.
 * @param id - The id of the new player that is to be created.
 * @return p - Player which is added to the list.
 */
Player *PlayerManager::createPlayer(int id) {
    auto *p = new Player(id);
    players.push_back(p);
    return p;
}

/**
 * Gets a player by its ID and adds it if it did not exist.
 * @param id - the ID of the player to retrieve.
 * @return p - An existing or newly created player.
 */
Player *PlayerManager::getPlayer(int id) {
    for (Player *p: this->getPlayers()) {
        if (p->getId() == id) {
            return p;
        }
    }
    return this->createPlayer(id);
}

/**
 * Setter for the list of players. These players can be both active and inactive.
 * Function for test purposes only.
 * @param players - List of pointers to player objects.
 */
void PlayerManager::setPlayers(std::list<Player *> players) {
    this->players = players;
}

/**
 * Destroys a player with a specific id.
 * Should be used when a player leaves the scene.
 * @param id - The id of the player that should be destroyed.
 */
void PlayerManager::destroyPlayer(int id) {
    list<Player *>::iterator it;
    Player *playerToDestroy = NULL;

    // Look for the player that is to be destroyed.
    for (it = players.begin(); it != players.end(); ++it) {
        if ((*it)->getId() == id) {
            playerToDestroy = (*it);
            break;
        }
    }

    // If there is a player to destroy, destroy it.
    if (playerToDestroy != NULL) {
        players.remove(playerToDestroy);
        delete playerToDestroy;
    }
}

/**
 * Destroys all players that are registered in PlayerManager.
 */
void PlayerManager::destroyAllPlayers() {
    for (Player *p : players) {
        delete p;
    }
    players.clear();
    this->idCounter = -1;
}

// GETTERS

/**
 * Generates a new id.
 * @return id - Unique string identifier.
 */
int PlayerManager::nextId() {
    this->idCounter = idCounter + 1;
    return idCounter;
}

/**
 * Getter for the list of players. These players can be both active and inactive.
 * @return players - List of pointers to player objects.
 */
std::list<Player *> PlayerManager::getPlayers() {
    return this->players;
}

/**
 * Getter for the number of players currently in the scene.
 * @return numberOfPlayers - The current number of players.
 */
int PlayerManager::numberOfPlayers() {
    return static_cast<int>(this->players.size());
}