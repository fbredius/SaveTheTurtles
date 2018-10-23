//
// Created by timbuntu on 8-5-18.
//

#ifndef GAME_TURTLEWALKFLEESTATE_H
#define GAME_TURTLEWALKFLEESTATE_H

#include "../Turtle.h"
#include "TurtleState.h"
#include "../../Obstacle.h"
#include <list>

/**
 * Class that denotes the walking state of turtles.
 *
 * In this state, the turtle runs away from nearby players.
 * When no player is nearby, it will walk around randomly.
 */
class TurtleWalkState : public TurtleState {

private:
    Turtle *turtle;
    double startRotation;
    double goalRotation;

    void transitionState();

public:
    explicit TurtleWalkState(Turtle *);

    void update(std::list<InputData *> inputData) override;

    void render(RenderWrapper *renderWrapper) override;

    void onCollide(Turtle &other) override;

    void onCollide(list<Player *> collidedPlayers, bool innerHitBox) override;

    void onCollide(ObjectiveArea &other) override;

    void onCollideObstacle() override;

    std::string getStateIdentifier() override;

    void move(InputData *inputData);

    void updateCounters(InputData *inputData);

    void turn();

};

#endif //GAME_TURTLEWALKSTATE_H