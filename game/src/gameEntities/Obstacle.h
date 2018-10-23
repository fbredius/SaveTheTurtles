//
// Created by timbuntu on 4-6-18.
//

#ifndef GAME_OBSTACLE_H
#define GAME_OBSTACLE_H

#include "../rendering/RenderWrapper.h"
#include "Entity.h"

/**
 * The obstacle object, the object turtles need to avoid.
 */
class Obstacle : public Entity {

private:
    double rotation;
    double alpha;
    bool fade;

public:
    Obstacle(double x, double y);

    void update(std::list<InputData *> inputList);

    void render(RenderWrapper *renderWrapper);

    // Getters

    double getRotation();

    void startFade();

    bool isFading() const;

    double getAlpha() const;

    int getId() const;
};

#endif //GAME_OBSTACLE_H