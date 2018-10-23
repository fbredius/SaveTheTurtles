//
// Created by thijmbuntu 24-5-18.
//

#ifndef GAME_TURTLERETURNINGSTATE_H
#define GAME_TURTLERETURNINGSTATE_H

#include "../Turtle.h"

/**
 * The turtle state when the turtle is returning to the spawn.
 */
class TurtleReturningState : public TurtleState {

private:
    Turtle *turtle;
    double startRotation;
    double goalRotation;
    double returnX;
    double returnY;
    bool initialRotateFinished;

public:
    explicit TurtleReturningState(Turtle *turtle);

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

    double getReturnX() const;

    double getReturnY() const;

    void setReturnX(double returnX);

    void setReturnY(double returnY);
};

#endif //GAME_TURTLERETURNINGSTATE_H
