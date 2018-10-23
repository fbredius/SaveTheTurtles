//
// Created by timbuntu on 8-5-18.
//

#ifndef GAME_GAMERUNNINGSTATE_H
#define GAME_GAMERUNNINGSTATE_H

#include "../TurtleManager.h"
#include "../PlayerManager.h"
#include "../GameManager.h"
#include "../../utility/CollisionHandler.h"
#include "../../gameEntities/Obstacle.h"
#include <cmath>

using namespace std;

/**
 * The running state, when the game is active.
 * This states ensures that collisions are detected and that all entities
 * interact correctly with eachother.
 */
class GameRunningState : public BasicState {
private:
    GameManager *gameManager;
    TurtleManager *turtleManager;
    PlayerManager *playerManager;
    CollisionHandler *collisionHandler;
    ObstacleManager *obstacleManager;

    bool fadeFinished;
    bool noActivity;
    double noActivityTimer;

    bool arrowsRendered;

    void init();

    void detectNonActivity(std::list<InputData *> inputList);

public:
    explicit GameRunningState(GameManager *gameManager);

    GameRunningState(GameManager *gameManager, PlayerManager *playerManager);

    GameRunningState(GameManager *gameManager, TurtleManager *turtleManager, PlayerManager *playerManager);

    GameRunningState(GameManager *gameManager, TurtleManager *turtleManager, PlayerManager *playerManager,
                     ObstacleManager *obstacleManager);

    GameRunningState();

    void update(std::list<InputData *> inputList) override;

    void render(RenderWrapper *renderWrapper) override;

    std::string getStateIdentifier() override;

    PlayerManager *getPlayerManager();

    TurtleManager *getTurtleManager();

    bool isFadeFinished() const;
};

#endif //GAME_GAMERUNNINGSTATE_H
