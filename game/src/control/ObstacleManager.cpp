//
// Created by maxim on 21-6-18.
//

#include "ObstacleManager.h"
#include <sstream>
#include <cmath>
#include <iostream>
#include "../storage/Variables.h"
#include "../utility/MathUtilities.h"

using namespace std;

/**
 * Constructor for the ObstacleManager.
 * This class maintains a list of obstacles and allows updating and rendering of these objects.
 * It maintains the specified amount of obstacles when obstacles are despawned and it allows removing obstacles.
 * This is used to implement the mechanic where obstacles respawn when hit by turtles.
 *
 * The constructor creates a set of obstacles.
 */
ObstacleManager::ObstacleManager() {
    this->active = false;
    this->turtleManager = nullptr;
    createObstacles();
}

/**
 * Update method for the ObstacleManager.
 * This method destroys obstacles that are faded out and it creates new obstacles if the number of obstacles is too
 * low.
 * @param inputList - The inputData used to get the frameTime, this is needed for smooth fading.
 */
void ObstacleManager::update(std::list<InputData *> inputList) {

    // Look for an obstacle that has been faded out and that should be faded out.
    Obstacle *obstacleToDestroy = NULL;
    list<Obstacle *>::iterator it;
    for (it = obstacles->begin(); it != obstacles->end(); ++it) {
        (*it)->update(inputList);
        if ((*it)->getAlpha() <= 0.0 && (*it)->isFading()) {
            obstacleToDestroy = (*it);
        }
    }

    // Destroy an obstacle if one was found and store its location in lastDestroyed.
    if (obstacleToDestroy != NULL) {
        lastDestroyed.first = obstacleToDestroy->getX();
        lastDestroyed.second = obstacleToDestroy->getY();
        obstacles->remove(obstacleToDestroy);
    }

    // If obstacles should be added, add one but only if the obstacleManager is active.
    if (obstacles->size() < (unsigned int) Variables::TOTAL_OBSTACLES && active) {
        addObstacle();
    }
}

/**
 * Render method for the ObstacleManager, this calls the render method for each Obstacle.
 * @param renderWrapper - The RenderWrapper that should be used for rendering.
 */
void ObstacleManager::render(RenderWrapper *renderWrapper) {
    // Render every obstacle.
    for (Obstacle *obstacle : *obstacles) {
        obstacle->render(renderWrapper);
    }
}

/**
 * Method that initializes the set of obstacles by using MathUtilities for generating random locations.
 */
void ObstacleManager::createObstacles() {

    // Get a set of points where obstacles should be created.
    list<pair<double, double>> *obstaclePoints = MathUtilities::generatePoints(
            MathUtilities::generateEllipse(Variables::ELLIPSE_DENSITY), Variables::ELLIPSE_DENSITY);

    // Add the obstacles on the specified points.
    auto *obstacles = new list<Obstacle *>;
    for (pair<double, double> point : *obstaclePoints) {
        obstacles->push_back(new Obstacle(point.first, point.second));
    }
    this->obstacles = obstacles;
}

/**
 * Add one obstacle, keeping in mind the locations of other obstacles, the last destroyed obstacle and the turtle
 * locations.
 */
void ObstacleManager::addObstacle() {
    auto *existing = new list<pair<double, double>>;
    list<Obstacle *>::iterator it;

    // Add the locations of all obstacles to the existing locations list.
    for (it = obstacles->begin(); it != obstacles->end(); ++it) {
        pair<double, double> position;
        position.first = (*it)->getX();
        position.second = (*it)->getY();
        existing->push_back(position);
    }

    // Add the locations of all turtles to the existing locations list, this prevents obstacles from spawning
    // inside turtles and instantly killing them.
    if (turtleManager != nullptr) {
        for (Turtle *turtle : turtleManager->getTurtles()) {
            pair<double, double> position;
            position.first = turtle->getX();
            position.second = turtle->getY();
            existing->push_back(position);
        }
        existing->push_back(lastDestroyed);
    }

    // Get a new position using MathUtilities.
    pair<double, double> *newPosition = MathUtilities::generatePointWithExistingPoints(
            MathUtilities::generateEllipse(Variables::ELLIPSE_DENSITY), existing, Variables::ELLIPSE_DENSITY);

    // Use the generated position to create an obstacle and add it.
    auto *newObstacle = new Obstacle(newPosition->first, newPosition->second);
    obstacles->push_back(newObstacle);
}

/**
 * Method for fading out all obstacles, this is used when going back to the Interactive Environment (IdleState).
 * Also set active to false to prevent obstacles from respawning.
 */
void ObstacleManager::fadeAll() {
    this->active = false;
    list<Obstacle *>::iterator it;
    for (it = obstacles->begin(); it != obstacles->end(); ++it) {
        (*it)->startFade();
    }
}

/**
 * Getter for the list of obstacles, used by the CollisionHandler.
 * @return obstacles - The currently active set of obstacles.
 */
list<Obstacle *> *ObstacleManager::getObstacles() const {
    return obstacles;
}

/**
 * Getter for the active boolean, this indicated whether the obstacle hitBoxes are activated.
 * This is false when not in the GameRunningState to prevent unwanted collisions.
 * @return active - The current active status of the ObstacleManager.
 */
bool ObstacleManager::isActive() const {
    return active;
}

/**
 * Setter for the active boolean.
 * Also resets the fade of all the obstacles to make sure that they fade in when the ObstacleManager is reactivated.
 * @param active - The active status that the ObstacleManager should be set to.
 */
void ObstacleManager::setActive(bool active) {
    ObstacleManager::active = active;
}

/**
 * Setter for the current TurtleManager, this is used to prevent obstacles from spawning inside turtles.
 * @param turtleManager - The currently active TurtleManager.
 */
void ObstacleManager::setTurtleManager(TurtleManager *turtleManager) {
    ObstacleManager::turtleManager = turtleManager;
}

/**
 * Setter for the obstacles list, should not be used when a game is in progress.
 */
void ObstacleManager::setObstacles(list<Obstacle *> *obstacles) {
    ObstacleManager::obstacles = obstacles;
}
