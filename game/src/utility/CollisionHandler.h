//
// Created by thijmen on 22-5-18.
//

#ifndef GAME_COLLISIONHANDLER_H
#define GAME_COLLISIONHANDLER_H

#include "../gameEntities/Player.h"
#include "../gameEntities/turtle/Turtle.h"
#include "../control/GameManager.h"
#include "../control/PlayerManager.h"
#include "../control/TurtleManager.h"
#include "../gameEntities/Obstacle.h"
#include "../storage/Variables.h"
#include "../gameEntities/ObjectiveArea.h"
#include "../control/ObstacleManager.h"

using namespace std;

/**
 * The CollisionHandler object, the object that handles collisions 
 * between GameEntities by detecting collisions and calling the appropriate
 * onCollide methods on the Turtle objects.
 */
class CollisionHandler {

private:
    vector<bool> addedScore;

    TurtleManager *turtleManager;

    PlayerManager *playerManager;

    ObstacleManager *obstacleManager;

    list<ObjectiveArea *> objectiveList;

public:
    CollisionHandler(PlayerManager *playerManager, TurtleManager *turtleManager, ObstacleManager *obstacleManager,
                     list<ObjectiveArea *> objectiveAreas);

    CollisionHandler(const CollisionHandler &collisionHandler);

    ~CollisionHandler();

    void handleCollision();

    bool checkTurtleCollisions(Turtle *turtle);

    bool checkPlayerCollisions(Turtle *turtle);

    bool checkObstacleCollisions(Turtle *turtle);

    bool checkObjectiveCollisions(Turtle *turtle);

    static bool checkHitBoxCollision(HitBox hitBox1, HitBox hitBox2);

};

#endif //GAME_COLLISIONHANDLER_H