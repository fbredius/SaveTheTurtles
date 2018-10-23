//
// Created by timbuntu on 8-5-18.
//

#ifndef GAME_TURTLE_H
#define GAME_TURTLE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "../../utility/BasicState.h"
#include "../../storage/HitBox.h"
#include "../Entity.h"

/**
 * The turtle object. This object contains several booleans that all indicate
 * small substates of the turtle, these are used for game logic and rendering.
 * The entire behaviour of the turtle can change depending on its state object.
 * Update and render calls are propagated to the state object.
 */
class Turtle : public Entity {

private:
    void init();

    BasicState *currentState;
    bool alive;
    bool colliding;
    bool returning;
    bool idle;
    bool roaming;
    bool inObjective;
    bool outOfScreen;
    bool interactable;
    bool resetting;
    bool againstWall;

    double direction;
    double speed;

    bool wiggling;
    double wiggleTimer;
    double wiggleFactor;
    double moveTimer;

    double alpha;

public:
    explicit Turtle();

    Turtle(const Turtle &obj);

    ~Turtle();

    void update(std::list<InputData *> inputList);

    void render(RenderWrapper *renderWrapper);

    void spawn();

    void hatch();

    void kill();

    double calculateDirection(double returnX, double returnY);

    // Getters
    BasicState *getCurrentState() const;

    bool isAlive() const;

    bool isReturning() const;

    bool isIdle() const;

    bool isRoaming() const;

    double getDirection() const;

    double getSpeed() const;

    bool isWiggling() const;

    double getWiggleTimer() const;

    double getWiggleFactor() const;

    double getMoveTimer() const;

    bool isColliding() const;

    bool isOutOfScreen() const;

    bool isInteractable() const;

    bool isResetting() const;

    double getAlpha() const;

    // Setters
    void setCurrentState(BasicState *currentState);

    void setId(int id);

    void setAlive(bool alive);

    void setReturning(bool returning);

    void setIdle(bool idle);

    void setRoaming(bool roaming);

    void setX(double x);

    void setY(double y);

    void setPosition(double x, double y);

    void setDirection(double direction);

    void setSpeed(double speed);

    void setInObjective(bool inObjective);

    void setColliding(bool collide);

    void setMoveTimer(double timer);

    void setWiggling(bool wiggling);

    void setWiggleTimer(double wiggleTime);

    void setWiggleFactor(double wiggleFactor);

    void setOutOfScreen();

    void setInteractable();

    void setResetting(bool value);

    void setAlpha(double alpha);

    bool isAgainstWall() const;
};

#endif //GAME_TURTLE_H
