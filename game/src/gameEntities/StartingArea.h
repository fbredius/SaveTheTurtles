//
// Created by maxim on 7-6-18.
//

#ifndef GAME_STARTINGAREA_H
#define GAME_STARTINGAREA_H

#include "../storage/HitBox.h"
#include "Player.h"
#include <map>
#include <list>

using namespace std;

class StartingArea {

private:
    map<int, double> playerTimers;
    int xMin;
    int xMax;
    int yMin;
    int yMax;
    double visibilityTimer;

    bool isInArea(double x, double y);

public:
    StartingArea(int xMin, int xMax, int yMin, int yMax);

    void updateTimers(list<Player *> players, double frameTime);

    void render(RenderWrapper *renderWrapper);

    bool isActivated();

};

#endif //GAME_STARTINGAREA_H