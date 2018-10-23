//
// Created by timbuntu on 8-5-18.
//

#ifndef GAME_TURTLEMANAGER_H
#define GAME_TURTLEMANAGER_H

#include <list>
#include "../gameEntities/turtle/Turtle.h"
#include <cmath>

/**
 * TurtleManager that manages all the turtles.
 * It propagates update and render calls to all its turtles.
 * Furthermore, it allows creation and deletion of turtles.
 * It also has methods that change the state of all turtles or that checks
 * whether all turtles are in a certain state. (e.g. for resetting)
 */
class TurtleManager {

private:
    std::list<Turtle *> turtles;
    int liveTurtles;
    int leftToSpawn;
    bool resetting;
    bool infiniteSpawn;

    list<pair<float, float>> toBeAnimated;

public:
    TurtleManager();

    void update(std::list<InputData *> inputList);

    void renderTurtles(RenderWrapper *renderWrapper);

    void resetTurtles();

    bool allTurtlesReturned();

    void diversifyStates();

    // Turtle Factory
    void createTurtle();

    void destroyTurtle(int id);

    bool checkSpawnClear(Turtle *checkSpawn);

    void startDeathAnimation(float x, float y);

    // Setters
    void setLiveTurtles(int liveTurtles);

    void setLeftToSpawn(int leftToSpawn);

    void setTurtles(std::list<Turtle *> turtles);

    void setInfiniteSpawn(bool infiniteSpawn);

    // Getters

    int getLiveTurtles();

    int getLeftToSpawn();

    std::list<Turtle *> getTurtles();
};

#endif //GAME_TURTLEMANAGER_H
