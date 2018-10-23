//
// Created by maxim on 15-5-18.
//

#include "Variables.h"
#include <SFML/Graphics.hpp>

using namespace sf;

/**
 * This class contains all the configurable variables that are used in the game.
 * No input validation is used since this is also not used in IMOVE configuration, however this could be a
 * future extension.
 */

// --- SFML Window Configuration ---
const std::string Variables::GAME_NAME = "Turtle Game";
const int Variables::PIXEL_WIDTH = 1920;
const int Variables::PIXEL_HEIGHT = 1080;
const unsigned int Variables::FRAMERATE_LIMIT = 120;
const double Variables::PIXELS_PER_METER = 425.79348754882812;
bool Variables::IMOVE_ACTIVE; // Automatically set.

// --- Debug Configuration ---
const double Variables::DEBUG_UPDATE_RATE = 0.5;
bool Variables::DEBUG_MODE = true;
bool Variables::DEBUG_SLOWMO_MODE = false;
double Variables::DEBUG_SLOWMO_FACTOR = 0.5;

// --- Gameplay Configuration ---
const int Variables::TOTAL_TURTLES = 15;
const int Variables::MAX_VISIBLE_TURTLES = 5;
int Variables::NUMBER_OF_PLAYERS = 3;
int Variables::DEAD_TURTLES = 0;
int Variables::ID_COUNTER = 0;
int Variables::SCORE_WIN_THRESHOLD = 8;

// -- GameLoop Configuration ---
const double Variables::NO_ACTIVITY_TIMEOUT = 3.0;
const double Variables::START_SCREEN_TIME = 10.0;
const double Variables::END_SCREEN_TIME = 8.0;
const double Variables::PERCENTAGE_OF_HATCHED_TURTLES = 1.0;
const double Variables::SCREEN_FADE_TIME = 1.0;
const bool Variables::SHOW_SPONSOR = false;
const bool Variables::SHOW_TUTORIAL = false;

// -- Player Configuration ---
// All real-world parameters: meters, seconds and meters/second.
const double Variables::PLAYER_FORCE_RADIUS = 0.5;
const double Variables::PLAYER_WALL_RADIUS = Variables::PLAYER_FORCE_RADIUS * 0.5;
const double Variables::PLAYER_TIMEOUT = 3.0;
const bool Variables::AUTO_DESTROY_PLAYERS = true;

// --- Turtle Configuration ---
// All real-world parameters: meters, seconds and meters/second.
double Variables::TURTLE_BORDER_OFFSET = 0.3;
const double Variables::EGG_SPAWN_TIME = 1.5;
const double Variables::TURTLE_WALK_SPEED = 0.6;
const double Variables::TURTLE_FORCE_SPEED = TURTLE_WALK_SPEED * 3.5;
const double Variables::TURTLE_IDLE_SPEED = 0.25;
const double Variables::TURTLE_ROAMING_SPEED = 0.125;
const double Variables::TURTLE_RESET_SPEED = 0.25;
const double Variables::TURTLE_OBJECTIVE_SPEED = 0.6;
const double Variables::TURTLE_BORDER_SPEED = 0.125;
const double Variables::TURTLE_RADIUS = 0.1;
const double Variables::WIGGLE_DURATION_IDLE = 0.4;
const double Variables::WIGGLE_OFFSET_IDLE = 0.2;
const double Variables::WIGGLE_DURATION_OBJECTIVE = 0.4;
const double Variables::WIGGLE_OFFSET_OBJECTIVE = 0.5;
const double Variables::WIGGLE_DURATION_RETURNING = 0.8;
const double Variables::WIGGLE_OFFSET_RETURNING = 0.1;
const double Variables::MOVE_DURATION_ROAMING = 1.0;
const double Variables::WIGGLE_DURATION_ROAMING = 0.4;
const double Variables::WIGGLE_OFFSET_ROAMING = 0.15;
const double Variables::MOVE_DURATION_WALK = 0.8;
const double Variables::WIGGLE_DURATION_WALK = 0.2;
const double Variables::WIGGLE_OFFSET_WALK = 0.6;
const double Variables::INTERACTION_ROTATE_TIME = 0.1;
// This variable is not in real-world values.
const int Variables::ROAMING_BUFFER = 50;

// -- Objective Configuration ---
const double Variables::OBJECTIVE_RADIUS = 0.9;
const double Variables::VISIBILITY_MARGIN = 100.0;
const double Variables::DESPAWN_MARGIN = 25.0;
const double Variables::ROAMING_NOGO_RADIUS = 1.5;

// --- Starting Area Configuration ---
// All real-world parameters: meters, seconds and meters/second.
const double Variables::START_GAME_TIME = 2.0;
const double Variables::START_AREA_WIDTH = 1.2;
const double Variables::START_AREA_HEIGHT = 1.0;
const double Variables::START_AREA_VISIBILITY_TIME = 0.1;

// --- Obstacle Configuration ---
int Variables::TOTAL_OBSTACLES = 4;
const double Variables::OBSTACLE_RADIUS = 0.13;
const double Variables::OBSTACLE_SPACING = 0.16667;
const double Variables::OBSTACLE_PLACING = 0.35;
const int Variables::ELLIPSE_DENSITY = 20;
const double Variables::OBSTACLE_FADE_SPEED = 1000.0;

// --- Trail Configuration ---
const double Variables::TRAIL_DURATION = 0.7;
const double Variables::TRAIL_FREQUENCY = 100.0;
const double Variables::TRAIL_FADE_SPEED = 0.05;

// --- GUI Configuration ---
const double Variables::GUI_SCORE_GAME = 0.17;
const double Variables::GUI_SCORE_FINISH = 0.17;

// --- Animation Configuration ---
const int Variables::TURTLE_ANIMATION_SPEED = 7;
const int Variables::OBSTACLE_ANIMATION_SPEED = 4;
const int Variables::DEATH_ANIMATION_SPEED = 4;
const double Variables::DEATH_ANIMATION_SIZE = 0.8;
const int Variables::SCORE_ANIMATION_SPEED = 4;
const double Variables::SCORE_ANIMATION_SIZE = 0.8;
const int Variables::ARROW_ANIMATION_SPEED = 10;
const double Variables::ARROW_ANIMATION_SIZE = 0.35;

// --- Debug Variables ---
// These should not be configured, they are changed during runtime.
// Updated from several game logic components to allow them to be drawn to the screen.
int Variables::DEBUG_PLAYER_COUNT = -1;
int Variables::DEBUG_TURTLE_COUNT = -1;
std::string Variables::DEBUG_GAME_STATE;
int Variables::DEBUG_TURTLES_LEFT_TO_SPAWN = -1;

// Global Utility Methods.

/**
 * Method that scales a real world parameters (meters) to pixels. Using the PIXELS_PER_METER variable.
 * @param meters - the number of meters.
 * @return pixels - the converted number of pixels.
 */
int Variables::scaleToPixels(double meters) {
    return static_cast<int>(PIXELS_PER_METER * meters);
}

/**
 * Function to get a new global ID.
 * @return the new id.
 */
int Variables::getNewId() {
    Variables::ID_COUNTER += 1;
    return Variables::ID_COUNTER;
}

// Score Methods.

int Variables::SCORE = 0;

/**
 * Getter for the score, used to render the score value.
 * @return Variables::SCORE - the current score value.
 */
int Variables::getScore() {
    return Variables::SCORE;
}

/**
 * Function to set a score to a new value.
 * @param score - The new updated score.
 */
void Variables::setScore(int score) {
    Variables::SCORE = score;
}

/**
 * Function to add a value to the score.
 * @param score - The score to be added.
 */
void Variables::addScore(int score) {
    Variables::SCORE += score;
}

/**
 * Function to reset the score to 0.
 */
void Variables::resetScore() {
    Variables::SCORE = 0;
}