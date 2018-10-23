//
// Created by thijmen on 6-6-18.
//

#ifndef GAME_TURTLEROAMINGSTATE_H
#define GAME_TURTLEROAMINGSTATE_H

#include "../../../utility/BasicState.h"
#include "../Turtle.h"
#include "../../../storage/Variables.h"
#include "TurtleState.h"
#include "../../Obstacle.h"

/**
 * The turtle state when the turtle is in the objective area.
 * The turtle is in this state when the player has pushed said turtle into any of the four objective areas.
 */
class TurtleRoamingState : public TurtleState {

private:
    Turtle *turtle;
    double startRotation;
    double goalRotation;
    double goalX;
    double goalY;

public:
    explicit TurtleRoamingState(Turtle *turtle);

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

    std::pair<double, double> getRandomLocation(double currentX, double currentY);

};

#endif //GAME_TURTLEROAMINGSTATE_H
