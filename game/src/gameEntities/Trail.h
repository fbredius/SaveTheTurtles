//
// Created by raoulesd on 5-6-18.
//

#ifndef GAME_TRAIL_H
#define GAME_TRAIL_H

#include <list>
#include "../rendering/RenderWrapper.h"
#include "TrailPoint.h"

/**
 * This class keeps track of a trail for the interactive environment. A trail consists of a list of points that form
 * the trail.
 */
class Trail {

private:
    std::list<TrailPoint *> points;
    std::list<TrailPoint *> toBeRemoved;

    double timer;

    void removePoints();

public:
    Trail();

    void render(RenderWrapper *renderWrapper);

    //Getters
    double getTime();

    std::list<TrailPoint *> getTrailPoints();

    //Setters

    void updatePoints(float time);

    void resetTimer();

    void update(double x, double y, int direction);
};

#endif //GAME_TRAIL_H