//
// Created by mariette on 25-5-18.
//

#ifndef GAME_TURTLEIDLESTATE_H
#define GAME_TURTLEIDLESTATE_H

#include "../Turtle.h"
#include "TurtleState.h"
#include "../../Obstacle.h"

/**
 * The idle state, when the turtle is in the spawn area.
 * While in this state the turtle will continuously 
 * pick random locations in the spawn area and move to those locations.
 */
class TurtleIdleState : public TurtleState {

private:
    Turtle *turtle;
    double startRotation;
    double goalRotation;
    double goalX;
    double goalY;

public:
    explicit TurtleIdleState(Turtle *turtle);

    void update(std::list<InputData *> inputList) override;

    void render(RenderWrapper *renderWrapper) override;

    void onCollide(Turtle &other) override;

    void onCollide(list<Player *> collidedPlayers, bool innerHitBox) override;

    void onCollide(ObjectiveArea &other) override;

    void onCollideObstacle() override;

    std::string getStateIdentifier() override;

    void move(InputData *inputData);

    void updateCounters(InputData *inputData);

    void turn();

    void changeGoalLocation();
};

#endif //GAME_TURTLEIDLESTATE_H