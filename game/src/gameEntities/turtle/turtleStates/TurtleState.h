//
// Created by maxim on 15-6-18.
//

#ifndef GAME_TURTLESTATE_H
#define GAME_TURTLESTATE_H

#include "../../../utility/BasicState.h"
#include "../Turtle.h"
#include "../../Player.h"
#include "../../ObjectiveArea.h"

/**
 * The superclass for all the turtle states.
 */
class TurtleState : public BasicState {

public:
    virtual void onCollide(Turtle &other) = 0;

    virtual void onCollide(list<Player *> collidedPlayers, bool innerHitBox) = 0;

    virtual void onCollide(ObjectiveArea &other) = 0;

    virtual void onCollideObstacle() = 0;

};

#endif //GAME_TURTLESTATE_H