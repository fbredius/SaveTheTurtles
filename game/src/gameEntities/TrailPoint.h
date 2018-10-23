//
// Created by raoulesd on 5-6-18.
//

#ifndef GAME_TRAILPOINT_H
#define GAME_TRAILPOINT_H

#include <list>

/**
 * This class keeps track of one TrailPoint, and keeps track of its opacity and the speed that this opacity is reduced.
 */
class TrailPoint {

private:
    double x;
    double y;
    int direction;
    double opacity;
    int textureId;

public:
    double fadeTimer;

    TrailPoint(double x, double y, int direction);

    //Getters
    double getX();

    double getY();

    int getDirection();

    double getOpacity();

    int getTextureId();

    //Setters
    void setX(int x);

    void setY(int y);

    void setDirection(int direction);

    void decreaseFadeTimer(double time);

};

#endif //GAME_TRAILPOINT_H