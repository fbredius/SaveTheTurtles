//
// Created by thijmen on 22-5-18.
//

#include "CollisionHandler.h"
#include "../control/gameStates/GameRunningState.h"
#include "../storage/Variables.h"
#include "../gameEntities/turtle/turtleStates/TurtleSpawnState.h"
#include "../gameEntities/turtle/turtleStates/TurtleObjectiveState.h"
#include "../gameEntities/turtle/turtleStates/TurtleWalkState.h"
#include "MathUtilities.h"
#include <cmath>
#include <iostream>
#include <utility>

/**
 * Constructor for the collision handler.
 * @param playerManager - Player manager of the collision handler.
 * @param turtleManager - Turtle manager of the collision handler.
 * @param obstacles - A list containing all the obstacles.
 * @param objectiveList - A list containing all the objectiveAreas.
 */
CollisionHandler::CollisionHandler(PlayerManager *playerManager, TurtleManager *turtleManager,
                                   ObstacleManager *obstacleManager, list<ObjectiveArea *> objectiveList)
        : addedScore(vector<bool>(turtleManager->getTurtles().size(), false)), objectiveList(objectiveList) {

    this->playerManager = playerManager;
    this->turtleManager = turtleManager;
    this->obstacleManager = obstacleManager;
}

/**
 * Copy-constructor for collision handler.
 * @param collisionHandler - Pointer to the to be copied collisionHandler.
 */
CollisionHandler::CollisionHandler(const CollisionHandler &collisionHandler) {
    playerManager = collisionHandler.playerManager;
    turtleManager = collisionHandler.turtleManager;
    obstacleManager = collisionHandler.obstacleManager;
}

/**
 * Destructor for de-allocating memory.
 */
CollisionHandler::~CollisionHandler() {
    delete this->playerManager;
    delete this->turtleManager;
}

/**
 * Method that handles all collision in the game by calling methods for turtle, player, obstacle and objective
 * collision.
 * This method should be called upon each frame to ensure that all game logic is updated correctly.
 */
void CollisionHandler::handleCollision() {
    list<Turtle *> turtleList = turtleManager->getTurtles();
    list<Turtle *>::iterator turtleIterator;

    // For each turtle, check whether it collides with other turtles, players, an obstacle or an objective.
    for (turtleIterator = turtleList.begin(); turtleIterator != turtleList.end(); ++turtleIterator) {
        checkTurtleCollisions((*turtleIterator));
        checkPlayerCollisions((*turtleIterator));
        if (obstacleManager->isActive()) {
            checkObstacleCollisions((*turtleIterator));
        }
        checkObjectiveCollisions((*turtleIterator));
    }
}

/**
 * Method that checks for a single turtle whether it collides with other turtles.
 * If it collides with another turtle then the onCollide methods are called on both turtles.
 * @param turtle - The turtle for which it is checked whether it collides with other turtles.
 * @return collision - Returns true if the turtle collides with any other turtle.
 */
bool CollisionHandler::checkTurtleCollisions(Turtle *turtle) {
    list<Turtle *>::iterator turtleIterator;
    list<Turtle *> turtleList = turtleManager->getTurtles();
    bool collision = false;

    HitBox hitBoxTurtle1 = turtle->getHitBox();
    auto *state1 = dynamic_cast<TurtleState *>(turtle->getCurrentState());

    // Check for collisions with other turtles.
    for (turtleIterator = turtleList.begin(); turtleIterator != turtleList.end(); ++turtleIterator) {

        // Skip this iteration if we are checking whether a turtle collides with itself.
        if (turtle->getId() == (*turtleIterator)->getId()) {
            continue;
        }

        HitBox hitBoxTurtle2 = (*turtleIterator)->getHitBox();

        if (checkHitBoxCollision(hitBoxTurtle1, hitBoxTurtle2)) {
            auto *state2 = dynamic_cast<TurtleState *>((*turtleIterator)->getCurrentState());
            state2->onCollide(*(*turtleIterator));
            state1->onCollide(*(*turtleIterator));
            collision = true;
        }
    }

    return collision;
}

/**
 * Method that checks for a single turtle whether it collides with players.
 * If it collides with a player then the onCollide method is called on the turtle.
 * Depending on whether the inner or the outer HitBox of the player is hit, a boolean will be set in the onCollide
 * method, this is used by the turtle to determine how to update its game logic.
 * The method exits upon the first collision that is detected.
 *
 * @param turtle - The turtle for which it is checked whether it collides with players.
 * @return collision - Returns true if the turtle collides with any player.
 */
bool CollisionHandler::checkPlayerCollisions(Turtle *turtle) {
    list<Player *>::iterator playerIterator;
    list<Player *> playerList = playerManager->getPlayers();

    // Failsafe for when a turtle state cannot be cast correctly.
    if (!dynamic_cast<TurtleState *>(turtle->getCurrentState())) {
        return false;
    }

    // Retrieve the hitBox from the turtle.
    HitBox hitBoxTurtle = turtle->getHitBox();

    list<Player *> collidedPlayers;
    bool innerCollision = false;

    // Iterate over all players.= and check for collisions.
    for (playerIterator = playerList.begin(); playerIterator != playerList.end(); ++playerIterator) {
        HitBox hitBoxPlayerOuter = (*playerIterator)->getOuterHitBox();
        HitBox hitBoxPlayerInner = (*playerIterator)->getHitBox();

        if (checkHitBoxCollision(hitBoxTurtle, hitBoxPlayerInner)) {
            // If the turtle collides with the innerHitBox then call the onCollide method with innerHitBox = true.
            innerCollision = true;
            collidedPlayers.push_back((*playerIterator));
        } else if (checkHitBoxCollision(hitBoxTurtle, hitBoxPlayerOuter)) {
            // Else if the turtle collides with the outerHitBox then call the onCollide method with innerHitBox = false.
            collidedPlayers.push_back((*playerIterator));
        }
    }

    // If there was a collision, call the onCollide method.
    if (!collidedPlayers.empty()) {
        auto *state = dynamic_cast<TurtleState *>(turtle->getCurrentState());
        state->onCollide(collidedPlayers, innerCollision);
        return true;
    }

    return false;
}

/**
 * Method that checks for a single turtle whether it collides with an obstacle.
 * If it collides with an obstacle then the onCollide method is called on the turtle and the method immediately exits.
 * Also updated the turtleManager to spawn new turtles if needed.
 * @param turtle - The turtle for which it is checked whether it collides with an obstacle.
 * @return collision - Returns true if the turtle collides with any obstacle.
 */
bool CollisionHandler::checkObstacleCollisions(Turtle *turtle) {
    list<Obstacle *>::iterator obstacleIterator;
    list<Obstacle *> *obstacleList = obstacleManager->getObstacles();
    bool collision = false;

    // Retrieve the hitBox and turtleState from the turtle.
    HitBox hitBoxTurtle = turtle->getHitBox();
    auto *state = dynamic_cast<TurtleState *>(turtle->getCurrentState());

    // If the obstacleManager is unusable during state transitions, do not attempt to check fo obstacle collisions.
    if (obstacleManager == nullptr || obstacleList == nullptr || obstacleList->empty()) {
        return false;
    }

    // Iterate over all obstacles.
    for (obstacleIterator = obstacleList->begin(); obstacleIterator != obstacleList->end(); ++obstacleIterator) {
        // Skip obstacles that are fading since they cannot collide anymore.
        if ((*obstacleIterator)->isFading()) {
            continue;
        }

        HitBox hitBoxObstacle = (*obstacleIterator)->getHitBox();

        // Call the onCollide method if there is a collision with an obstacle.
        // Also update the turtleManager to spawn a new turtle if needed.
        if (checkHitBoxCollision(hitBoxObstacle, hitBoxTurtle)) {
            (*obstacleIterator)->startFade();
            state->onCollideObstacle();
            turtleManager->setLiveTurtles(turtleManager->getLiveTurtles() - 1);
            turtleManager->destroyTurtle(turtle->getId());
            turtleManager->createTurtle();
            turtleManager->startDeathAnimation(turtle->getX(), turtle->getY());
            return true;
        }
    }

    return collision;
}

/**
 * Method that checks for a single turtle whether it collides with an objective.
 * If it collides with an objective then the onCollide method is called on the turtle and the method immediately exits.
 * @param turtle - The turtle for which it is checked whether it collides with an objective.
 * @return collision - Returns true if the turtle collides with any objective.
 */
bool CollisionHandler::checkObjectiveCollisions(Turtle *turtle) {
    list<ObjectiveArea *>::iterator objectiveIterator;
    bool collision = false;

    // Retrieve the hitBox and turtleState from the turtle.
    HitBox hitBoxTurtle = turtle->getHitBox();
    auto *state = dynamic_cast<TurtleState *>(turtle->getCurrentState());

    // Iterate over all objectives.
    for (objectiveIterator = objectiveList.begin(); objectiveIterator != objectiveList.end(); ++objectiveIterator) {
        HitBox hitBoxObjective = (*objectiveIterator)->getHitBox();

        // Call the onCollide method if there is a collision with an objective.
        if (checkHitBoxCollision(hitBoxObjective, hitBoxTurtle)) {
            state->onCollide(*(*objectiveIterator));
            return true;
        }
    }

    return collision;
}

/**
 * The general collision calculation function between two circular hit-boxes.
 *
 * @param hitBox1 - The first hit-box object the collision is calculated with.
 * @param hitBox2 - The second hit-box object the collision is calculated with.
 * @return colliding - The status if the two hit-boxes are colliding/overlapping.
 */
bool CollisionHandler::checkHitBoxCollision(HitBox hitBox1, HitBox hitBox2) {
    // Minimum range the two hitBoxes can be before they collide.
    double minRange = hitBox1.getRadius() + hitBox2.getRadius();

    // X and Y distance between the two hitBoxes.
    double distanceX = fabs(hitBox1.getX() - hitBox2.getX());
    double distanceY = fabs(hitBox1.getY() - hitBox2.getY());

    // Total distance between the two hitBoxes using the pythagorean theorem.
    double distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

    return distance < minRange;
}