//
// Created by maxim on 5-6-18.
//

#ifndef GAME_GAMERESETSTATE_H
#define GAME_GAMERESETSTATE_H

#include "../../utility/BasicState.h"
#include "../GameManager.h"
#include "../TurtleManager.h"
#include "../../utility/CollisionHandler.h"

/**
 * The game reset state.
 * In this state the game gradually resets by moving turtles back to spawn.
 * This happens when no players are active in the playing field.
 */
class GameResetState : public BasicState {
private:
    GameManager *gameManager;
    TurtleManager *turtleManager;
    PlayerManager *playerManager;
    ObstacleManager *obstacleManager;

public:
    GameResetState(GameManager *gameManager, TurtleManager *turtleManager, PlayerManager *playerManager,
                   ObstacleManager *obstacleManager);

    void update(std::list<InputData *> inputList) override;

    void render(RenderWrapper *renderWrapper) override;

    std::string getStateIdentifier() override;

    PlayerManager *getPlayerManager() const;
};


#endif //GAME_GAMERESETSTATE_H
