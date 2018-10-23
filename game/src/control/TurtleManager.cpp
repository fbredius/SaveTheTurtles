//
// Created by timbuntu on 8-5-18.
//

#include "TurtleManager.h"
#include "../storage/Variables.h"
#include "../gameEntities/turtle/turtleStates/TurtleIdleState.h"
#include "../gameEntities/turtle/turtleStates/TurtleSpawnState.h"
#include "../gameEntities/turtle/turtleStates/TurtleReturningState.h"
#include "../gameEntities/turtle/turtleStates/TurtleObjectiveState.h"
#include "../gameEntities/turtle/turtleStates/TurtleWalkState.h"
#include "../utility/MathUtilities.h"
#include <iostream>
#include <cmath>

using namespace std;

/**
 * Constructor for the TurtleManager.
 */
TurtleManager::TurtleManager() {
    this->liveTurtles = 0;
    this->leftToSpawn = Variables::TOTAL_TURTLES;
    this->resetting = false;
    this->infiniteSpawn = false;
}

/**
 * Updates all turtles every tick and destroys turtles that have been scored.
 *
 * @param inputList - The list of input types.
 */
void TurtleManager::update(std::list<InputData *> inputList) {

    list<Turtle *>::iterator it;
    Turtle *toDestroy = NULL;

    // Iterate over all turtles.
    // Update each turtle and remove turtles that are out of the screen and thus brought to the objective area.
    for (it = turtles.begin(); it != turtles.end(); ++it) {
        if ((*it)->isOutOfScreen()) {
            /*
             * As we cannot edit a list we are iterating over, we can only delete 1 turtle per iteration.
             * Luckily the chances of 2 or more turtles going out of the screen on the same tick is very slim, and even
             * if it were the case, it wouldn't affect performance.
             */
            toDestroy = (*it);
        } else {
            (*it)->update(inputList);
        }
    }
    if (toDestroy != NULL) {
        destroyTurtle(toDestroy->getId());
    }

    if (turtles.size() < (unsigned int) Variables::MAX_VISIBLE_TURTLES) {
        createTurtle();
    }

    // Set the DEBUG variable for the number of turtles.
    Variables::DEBUG_TURTLE_COUNT = static_cast<int>(turtles.size());
}

// LCOV_EXCL_START
/**
 * Renders everything the turtle manager manages.
 * @param renderWrapper - Wrapper for the render window.
 */
void TurtleManager::renderTurtles(RenderWrapper *renderWrapper) {
    list<Turtle *>::iterator it;
    // Iterate over all turtles and render each one.
    for (it = turtles.begin(); it != turtles.end(); ++it) {
        (*it)->render(renderWrapper);
    }

    // Starts death animations for turtles that have just died.
    list<pair<float, float>> toBeRemoved;
    for (pair<float, float> pos : toBeAnimated) {
        renderWrapper->startDeathAnimation(pos.first, pos.second);
        toBeRemoved.push_back(pos);
    }
    for (pair<float, float> pos : toBeRemoved) {
        toBeAnimated.remove(pos);
    }

    // Comment this out to have a visual representation of the spawn area.
//    window->draw(spawnArea);
}
// LCOV_EXCL_STOP

/**
 * Adds a pair of coorindates to the list, so a death animation will be started
 * next iteration at that location.
 * @param x - The x coordinate of the animation.
 * @param y - The y coordinate of the animation.
 */
void TurtleManager::startDeathAnimation(float x, float y) {
    toBeAnimated.push_back(pair<float, float>(x, y));
}

/**
 * Put all turtles in the TurtleReturningState to make them move back to the spawn for the natural reset.
 */
void TurtleManager::resetTurtles() {
    list<Turtle *>::iterator it;
    // Iterate over all turtles and put them in the returning state.
    for (it = turtles.begin(); it != turtles.end(); ++it) {
        // We only want to reset turtles that are already active in the game and that have not been scored yet.
        if (!dynamic_cast<TurtleSpawnState *>((*it)->getCurrentState())
            && !dynamic_cast<TurtleObjectiveState *>((*it)->getCurrentState())) {
            (*it)->setCurrentState(new TurtleReturningState((*it)));
            (*it)->setSpeed(Variables::scaleToPixels(Variables::TURTLE_RESET_SPEED));
            (*it)->setResetting(true);
        }
    }

    // Indicate that a reset is in progress.
    resetting = true;
}

/**
 * Checks whether all turtles that were put in the TurtleReturningState are actually in spawn now.
 *
 * This should only be used when resetTurtles was called first since TurtleWalkState does not necessarily mean
 * that a turtle is in the spawn area. However, it does mean that if the turtle was in the ReturningState first.
 * @return allReturned - which is true if all turtles have returned.
 */
bool TurtleManager::allTurtlesReturned() {

    // If we are not resetting at all then there is no use in calling this method, thus we return false by default.
    if (!resetting) {
        return false;
    }

    bool allReturned = true;

    list<Turtle *>::iterator it;
    // Iterate over all turtles and check if they are in spawn.
    for (it = turtles.begin(); it != turtles.end(); ++it) {
        if (!MathUtilities::isInSpawn(static_cast<int>((*it)->getX()), static_cast<int>((*it)->getY()))) {
            return false;
        }
    }

    // Set resetting back to false since we have finished the reset.
    resetting = false;

    return allReturned;
}

/**
 * Method that changes the state of half of the turtles to the TurtleIdleState.
 * This is used from the GameIdleState to make the Interactive Environment more interesting.
 */
void TurtleManager::diversifyStates() {

    int numberOfTurtlesToChange = static_cast<int>(turtles.size() * Variables::PERCENTAGE_OF_HATCHED_TURTLES);
    int turtlesChanged = 0;

    list<Turtle *>::iterator it;
    // Iterate over turtles and change their states until enough turtles were changed.
    for (it = turtles.begin(); it != turtles.end(); ++it) {

        // Hatch the turtle to make it walk randomly in the spawn area.
        (*it)->hatch();

        // Check whether enough turtles were changed.
        turtlesChanged++;
        if (turtlesChanged >= numberOfTurtlesToChange) {
            break;
        }
    }
}

// Turtle Factory

/**
 * Creates a turtle with a fresh id and adds it to the turtles list.
 */
void TurtleManager::createTurtle() {
    if (liveTurtles < Variables::MAX_VISIBLE_TURTLES && (leftToSpawn > 0 || infiniteSpawn)) {
        bool collide = true;
        auto checkSpawn = new Turtle();

        while (collide) {
            collide = checkSpawnClear(checkSpawn);

            // Respawn turtles if they are colliding within the spawn area.
            if (collide) {
                checkSpawn->spawn();
            }
        }

        // If a safe spawn location has been found, add this turtle to the turtles list.
        turtles.push_back(checkSpawn);
        setLiveTurtles(getLiveTurtles() + 1);
        setLeftToSpawn(getLeftToSpawn() - 1);
    }
}

/**
 * Destroys a turtle with a specific id.
 * Should be used when a turtle leaves the scene.
 * @param id - The id of the turtle that should be destroyed.
 */
void TurtleManager::destroyTurtle(int id) {
    list<Turtle *>::iterator it;
    Turtle *turtleToDestroy = NULL;

    // Look for the turtle that is to be destroyed.
    for (it = turtles.begin(); it != turtles.end(); ++it) {
        if ((*it)->getId() == id) {
            turtleToDestroy = (*it);
            break;
        }
    }

    // If there is a turtle to destroy, destroy it.
    if (turtleToDestroy != NULL) {
        turtles.remove(turtleToDestroy);
    }
    // Reduce the amount of alive turtles.
    setLiveTurtles(getLiveTurtles() - 1);
}

/**
 * Check if the given turtle is trying to spawn on top of another turtle.
 * @param checkSpawn - The turtle to be checked.
 * @return isSpawnClear - The status if the spawn location of the turtle is clear.
 */
bool TurtleManager::checkSpawnClear(Turtle *checkSpawn) {
    bool isSpawnClear = false;
    list<Turtle *>::iterator turtleIterator;

    // Check the collisions for each individual turtle.
    for (turtleIterator = turtles.begin(); turtleIterator != turtles.end(); ++turtleIterator) {
        if ((*turtleIterator)->getId() != checkSpawn->getId()) {
            // Minimum range the two turtles can be before they collide.
            float minRange = static_cast<float>((*turtleIterator)->getRadius() + (*checkSpawn).getRadius());

            // X and Y distance between the turtles
            float distancex = static_cast<float>(fabs((*turtleIterator)->getX() - checkSpawn->getX()));
            float distancey = static_cast<float>(fabs((*turtleIterator)->getY() - checkSpawn->getY()));

            // Total distance between turtles using the pythagorean theorem.
            float distance = sqrtf(pow(distancex, 2) + pow(distancey, 2));

            // If the distance of two objects is shorter than the minimum range, they are colliding.
            if (distance < minRange) {
                isSpawnClear = true;
            }
        }
    }

    return isSpawnClear;
}

// Setters

/**
 * Set the amount of live turtles.
 * @param liveTurtles - The amount of live turtles.
 */
void TurtleManager::setLiveTurtles(int liveTurtles) {
    this->liveTurtles = liveTurtles;
}

/**
 * Set the amount of turtles left to spawn.
 * @param liveTurtles - The amount of turtles left to spawn.
 */
void TurtleManager::setLeftToSpawn(int leftToSpawn) {
    this->leftToSpawn = leftToSpawn;
    Variables::DEBUG_TURTLES_LEFT_TO_SPAWN = leftToSpawn;
}

/**
 * Function for testing purposes.
 * Set the turtles list.
 * @param turtles - The new turtles list.
 */
void TurtleManager::setTurtles(std::list<Turtle *> turtles) {
    this->turtles = turtles;
}

/**
 * Setter for the infinite spawn boolean.
 * This is used from the TurtleIdleState to ensure that turtles keep spawning in the interactive environment.
 * @param infiniteSpawn - a boolean that uncaps the number of turtle spawns if true.
 */
void TurtleManager::setInfiniteSpawn(bool infiniteSpawn) {
    TurtleManager::infiniteSpawn = infiniteSpawn;
}

// Getters

/**
 * Return the amount of turtles that are in the current game.
 * @return liveTurtles - The amount of live turtles.
 */
int TurtleManager::getLiveTurtles() {
    return liveTurtles;
}

/**
 * Return the amount of turtles that are left to spawn.
 * @return leftToSpawn - The amount of turtles left to spawn.
 */
int TurtleManager::getLeftToSpawn() {
    return leftToSpawn;
}

/**
 * Function that returns the list of turtles.
 * @return turtles - The list of turtles.
 */
std::list<Turtle *> TurtleManager::getTurtles() {
    return this->turtles;
}