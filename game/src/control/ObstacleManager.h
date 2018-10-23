//
// Created by maxim on 21-6-18.
//

#ifndef GAME_OBSTACLEMANAGER_H
#define GAME_OBSTACLEMANAGER_H

#include "../rendering/RenderWrapper.h"
#include "../gameEntities/Obstacle.h"
#include "TurtleManager.h"

/**
 * ObstacleManager that handles the game obstacles.
 * It allows destroying and fading of obstacles once they are hit.
 * And it makes sure that new obstacles spawn at correct locations once
 * the number of obstacles is too low.
 */
class ObstacleManager {
public:
    list<Obstacle *> *obstacles;
    TurtleManager *turtleManager;
    pair<double, double> lastDestroyed;
    bool active;

    ObstacleManager();

    void update(std::list<InputData *> inputList);

    void render(RenderWrapper *renderWrapper);

    void fadeAll();

    list<Obstacle *> *getObstacles() const;

    bool isActive() const;

    void setActive(bool active);

    void setTurtleManager(TurtleManager *turtleManager);

    void setObstacles(list<Obstacle *> *obstacles);


private:
    void createObstacles();

    void addObstacle();
};


#endif //GAME_OBSTACLEMANAGER_H
