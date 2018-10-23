//
// Created by thijmen on 14-5-18.
//

#ifndef GAME_PLAYERMANAGER_H
#define GAME_PLAYERMANAGER_H

#include <list>
#include "../gameEntities/Player.h"
#include "../storage/InputData.h"

/**
 * PlayerManager that manages all the players.
 * It propagates update and render calls to all the players.
 * And it allows creating/deletion of players.
 * Finally, it checks whether players are inactive and whether they should
 * be removed.
 */
class PlayerManager {

private:
    std::list<Player *> players;
    int idCounter;

public:
    PlayerManager();

    void update(std::list<InputData *> inputList);

    void renderPlayers(RenderWrapper *renderWrapper);

    void createPlayer();

    Player *createPlayer(int id);

    Player *getPlayer(int id);

    void setPlayers(std::list<Player *> players);

    // Setters
    void destroyPlayer(int id);

    void destroyAllPlayers();

    // Getters
    int nextId();

    std::list<Player *> getPlayers();

    int numberOfPlayers();

};

#endif //GAME_PLAYERMANAGER_H
