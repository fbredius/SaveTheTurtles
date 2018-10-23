//
// Created by maxim on 7-6-18.
//

#include <iostream>
#include "StartingArea.h"
#include "../storage/Variables.h"

/**
 * Constructor for a StartingArea object. This is an area that a player can stand in for some amount of time in order
 * to start the game.
 *
 * @param xMin - The x coordinate of the boundary on the left.
 * @param xMax - The x coordinate of the boundary on the right.
 * @param yMin - the y coordinate of the lower boundary.
 * @param yMax - the y coordinate of the upper boundary.
 */
StartingArea::StartingArea(int xMin, int xMax, int yMin, int yMax) {
    this->xMin = xMin;
    this->xMax = xMax;
    this->yMin = yMin;
    this->yMax = yMax;
    this->visibilityTimer = 0.0;
}

/**
 * Method that maintains timers for all Players that entered the StartingArea.
 * If any of these Players are in the area for long enough then the game starts.
 * If a Player leaves the area then its timer is reset.
 *
 * @param players - All the players in the game, a subset of this will actually be in the StartingArea.
 * @param frameTime - The time that passed during this frame, used for updating the timers.
 */
void StartingArea::updateTimers(list<Player *> players, double frameTime) {
    // The StartingArea becomes visible after players have been in the playing field for some time.
    // The visibilityTimer tracks whether there is enough activity to display the StartingArea as it is not visible
    // by default.
    if (players.empty()) {
        visibilityTimer = 0.0;
    } else {
        visibilityTimer += frameTime;
    }

    // If there was not enough activity yet then do not activate activate the StartingArea player updates.
    if (visibilityTimer < Variables::START_AREA_VISIBILITY_TIME) {
        playerTimers.clear();
        return;
    }

    // Iterate over all players.
    list<Player *>::iterator it;
    for (it = players.begin(); it != players.end(); ++it) {

        // The unique id of the player is used as a key in the map.
        int key = (*it)->getId();

        // Check whether the player is in the StartingArea.
        if (isInArea((*it)->getX(), (*it)->getY())) {

            // If a player is in the starting area then update or create its timer.
            if (playerTimers.count(key)) {
                playerTimers[key] = playerTimers[key] + frameTime;
            } else {
                playerTimers[key] = frameTime;
            }

        } else if (playerTimers.count(key)) {
            // If the player is not in the area but is in the map, then erase its entry from the map.
            playerTimers.erase(key);
        }
    }
}

// LCOV_EXCL_START
/**
 * Method that renders a StartingArea at its location by calling a method on the RenderWrapper.
 * The StartingArea only becomes visible once there has been enough activity, this is tracked by the visibilityTimer.
 * @param renderWrapper - The RenderWrapper that renders to the SFML window.
 */
void StartingArea::render(RenderWrapper *renderWrapper) {
    if (visibilityTimer >= Variables::START_AREA_VISIBILITY_TIME) {
        renderWrapper->renderStartingArea(xMin, xMax, yMin, yMax);
    }
}
// LCOV_EXCL_STOP

/**
 * Method that checks whether the StartingArea is activated.
 * This is done by iterating over the map and checking if the timer of any player is high enough.
 * @return Returns true if the game can be started.
 */
bool StartingArea::isActivated() {

    map<int, double>::iterator it;
    for (it = playerTimers.begin(); it != playerTimers.end(); ++it) {
        if ((*it).second >= Variables::START_GAME_TIME) {
            return true;
        }
    }

    return false;
}

/**
 * Method that checks whether a point is in the StartingArea.
 *
 * @param x - The x coordinate of the entity that could be in the StartingArea.
 * @param y - The y coordinate of the entity that could be in the StartingArea.
 * @return Returns true if the point is in the StartingArea.
 */
bool StartingArea::isInArea(double x, double y) {
    return x >= xMin && x <= xMax && y >= yMin && y <= yMax;
}