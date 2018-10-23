//
// Created by thijmen on 29-5-18.
//

#ifndef GAME_TURTLEOBJECTIVESTATE_H
#define GAME_TURTLEOBJECTIVESTATE_H

#include "../../../utility/BasicState.h"
#include "../Turtle.h"
#include "../../../storage/Variables.h"
#include "../../ObjectiveArea.h"
#include "TurtleState.h"
#include "../../Obstacle.h"

/**
 * The turtle state when the turtle is in the objective area.
 * The turtle is in this state when the player has pushed said turtle into any of the four objective areas.
 */
class TurtleObjectiveState : public TurtleState {

private:
    Turtle *turtle;
    double startRotation;
    double goalRotation;
    ObjectiveArea objectiveArea = ObjectiveArea(0, 0, 0, 0);
    bool animationPlayed;

public:
    explicit TurtleObjectiveState(Turtle *turtle, ObjectiveArea objectiveArea);

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

    double calculateAlpha();
};


#endif //GAME_TURTLEOBJECTIVESTATE_H
