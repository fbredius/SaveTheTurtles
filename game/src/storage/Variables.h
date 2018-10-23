//
// Created by maxim on 15-5-18.
//

#ifndef GAME_VARIABLES_H
#define GAME_VARIABLES_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Variables {

public:

    // --- SFML Window Configuration ---
    static const std::string GAME_NAME;
    static const int PIXEL_WIDTH;
    static const int PIXEL_HEIGHT;
    static const unsigned int FRAMERATE_LIMIT;
    static const double PIXELS_PER_METER;
    static bool IMOVE_ACTIVE; // Automatically set.

    // --- Debug Configuration ---
    static const double DEBUG_UPDATE_RATE;
    static bool DEBUG_MODE;
    static bool DEBUG_SLOWMO_MODE;
    static double DEBUG_SLOWMO_FACTOR;

    // --- Gameplay Configuration ---
    static const int TOTAL_TURTLES;
    static const int MAX_VISIBLE_TURTLES;
    static int NUMBER_OF_PLAYERS;
    static int DEAD_TURTLES;
    static int ID_COUNTER;
    static int SCORE_WIN_THRESHOLD;

    // -- GameLoop Configuration ---
    static const double NO_ACTIVITY_TIMEOUT;
    static const double START_SCREEN_TIME;
    static const double END_SCREEN_TIME;
    static const double PERCENTAGE_OF_HATCHED_TURTLES;
    static const double SCREEN_FADE_TIME;
    static const bool SHOW_SPONSOR;
    static const bool SHOW_TUTORIAL;

    // -- Player Configuration ---
    // All real-world parameters: meters and meters/second.
    static const double PLAYER_FORCE_RADIUS;
    static const double PLAYER_WALL_RADIUS;
    static const double PLAYER_TIMEOUT;
    static const bool AUTO_DESTROY_PLAYERS;

    // --- Turtle Configuration ---
    // All real-world parameters: meters and meters/second.
    static double TURTLE_BORDER_OFFSET;
    static const double EGG_SPAWN_TIME;
    static const double TURTLE_WALK_SPEED;
    static const double TURTLE_FORCE_SPEED;
    static const double TURTLE_IDLE_SPEED;
    static const double TURTLE_ROAMING_SPEED;
    static const double TURTLE_RESET_SPEED;
    static const double TURTLE_OBJECTIVE_SPEED;
    static const double TURTLE_BORDER_SPEED;
    static const double TURTLE_RADIUS;
    static const double WIGGLE_DURATION_IDLE;
    static const double WIGGLE_OFFSET_IDLE;
    static const double WIGGLE_DURATION_OBJECTIVE;
    static const double WIGGLE_OFFSET_OBJECTIVE;
    static const double WIGGLE_DURATION_RETURNING;
    static const double WIGGLE_OFFSET_RETURNING;
    static const double MOVE_DURATION_ROAMING;
    static const double WIGGLE_DURATION_ROAMING;
    static const double WIGGLE_OFFSET_ROAMING;
    static const double MOVE_DURATION_WALK;
    static const double WIGGLE_DURATION_WALK;
    static const double WIGGLE_OFFSET_WALK;
    static const double INTERACTION_ROTATE_TIME;
    // This variable is not in real-world values.
    static const int ROAMING_BUFFER;

    // -- Objective Configuration ---
    static const double OBJECTIVE_RADIUS;
    static const double VISIBILITY_MARGIN;
    static const double DESPAWN_MARGIN;
    static const double ROAMING_NOGO_RADIUS;

    // --- Starting Area Configuration ---
    // All real-world parameters: meters and meters/second.
    static const double START_GAME_TIME;
    static const double START_AREA_WIDTH;
    static const double START_AREA_HEIGHT;
    static const double START_AREA_VISIBILITY_TIME;

    // --- Obstacle Configuration ---
    static int TOTAL_OBSTACLES;
    static const double OBSTACLE_RADIUS;
    static const double OBSTACLE_SPACING;
    static const double OBSTACLE_PLACING;
    static const int ELLIPSE_DENSITY;
    static const double OBSTACLE_FADE_SPEED;

    // --- Trail Configuration ---
    static const double TRAIL_DURATION;
    static const double TRAIL_FREQUENCY;
    static const double TRAIL_FADE_SPEED;

    // --- GUI Configuration ---
    static const double GUI_SCORE_GAME;
    static const double GUI_SCORE_FINISH;

    // --- Animation Configuration ---
    static const int TURTLE_ANIMATION_SPEED;
    static const int OBSTACLE_ANIMATION_SPEED;
    static const int DEATH_ANIMATION_SPEED;
    static const double DEATH_ANIMATION_SIZE;
    static const int SCORE_ANIMATION_SPEED;
    static const double SCORE_ANIMATION_SIZE;
    static const int ARROW_ANIMATION_SPEED;
    static const double ARROW_ANIMATION_SIZE;

    // --- Debug Variables ---
    // These should not be configured, they are changed during runtime.
    // Updated from several game logic components to allow them to be drawn to the screen.
    static int DEBUG_PLAYER_COUNT;
    static int DEBUG_TURTLE_COUNT;
    static std::string DEBUG_GAME_STATE;
    static int DEBUG_TURTLES_LEFT_TO_SPAWN;

    // Global Utility Methods
    static int scaleToPixels(double meters);

    static int getNewId();

    // Score Methods
    static int SCORE;

    static int getScore();

    static void setScore(int score);

    static void addScore(int score);

    static void resetScore();
};

#endif //GAME_VARIABLES_H