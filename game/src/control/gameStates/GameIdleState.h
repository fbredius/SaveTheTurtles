//
// Created by timbuntu on 8-5-18.
//

#ifndef GAME_GAMEIDLESTATE_H
#define GAME_GAMEIDLESTATE_H

#include <list>
#include "../../gameEntities/StartingArea.h"
#include "../GameManager.h"
#include "../TurtleManager.h"
#include "../../utility/CollisionHandler.h"

/**
 * The idle state, before the game begins.
 */
class GameIdleState : public BasicState {
private:
    GameManager *gameManager;
    TurtleManager *turtleManager;
    PlayerManager *playerManager;
    CollisionHandler *collisionHandler;
    StartingArea *startingArea;

    void init();

public:
    explicit GameIdleState(GameManager *gameManager);

    GameIdleState(GameManager *gameManger, TurtleManager *turtleManager, PlayerManager *playerManager);

    void update(std::list<InputData *> inputVector) override;

    void render(RenderWrapper *renderWrapper) override;

    std::string getStateIdentifier() override;

    void startGame();

    PlayerManager *getPlayerManager() const;

};

#endif //GAME_GAMEIDLESTATE_H
