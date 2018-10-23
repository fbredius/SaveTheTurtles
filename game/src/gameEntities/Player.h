//
// Created by thijmen on 14-5-18.
//

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "../rendering/RenderWrapper.h"
#include "../storage/HitBox.h"
#include "Trail.h"
#include "Entity.h"

/**
 * The player object, IMOVE persons are coupled to an instance of a player.
 * The player has two hitboxes, the inner hitBox ensures that no turtles can
 * move under the feet of a person.
 * The object also maintains a (dust) trail as a visual addition to the game.
 * Finally, each player object has a counter to detect inactive players.
 */
class Player : public Entity {

private:
    double direction;
    double forceRange;

    bool active;
    bool forceColliding;
    bool wallColliding;
    Trail *trail;
    double timeOutTimer;

public:
    explicit Player(int id);

    void update(double inputX, double inputY, int type);

    void updateTrail(float time);

    void render(RenderWrapper *renderWrapper);

    // Getters

    double getDirection();

    bool isActive();

    bool isForceColliding();

    bool isWallColliding();

    HitBox getOuterHitBox();

    Trail *getTrail();

    // Setters
    void setX(double x);

    void setY(double y);

    void setDirection(double direction);

    void setActive(bool status);

    void setForceColliding(bool forceCollide);

    void setWallColliding(bool wallCollide);

    double getTimeOutTimer() const;

    void setTimeOutTimer(double timeOutTimer);

};

#endif //GAME_PLAYER_H