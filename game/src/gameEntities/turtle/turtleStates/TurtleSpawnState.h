//
// Created by timbuntu on 8-5-18.
//

#ifndef GAME_TURTLESPAWNSTATE_H
#define GAME_TURTLESPAWNSTATE_H

#include "../Turtle.h"
#include "TurtleState.h"
#include "../../Obstacle.h"

/**
 * The turtle state when the turtle has not spawned yet.
 */
class TurtleSpawnState : public TurtleState {

private:
    Turtle *turtle;
    double timer;
    double alpha;

public:
    explicit TurtleSpawnState(Turtle *turtle);

    void update(std::list<InputData *> inputList) override;

    void render(RenderWrapper *renderWrapper) override;

    void onCollide(Turtle &other) override;

    void onCollide(list<Player *> collidedPlayers, bool innerHitBox) override;

    void onCollide(ObjectiveArea &other) override;

    void onCollideObstacle() override;

    std::string getStateIdentifier() override;

    double getTimer() const;

    double getAlpha() const;

};

#endif //GAME_TURTLESPAWNSTATE_H
