//
// Created by timbuntu on 8-6-18.
//

#ifndef GAME_ANIMATIONHANDLER_H
#define GAME_ANIMATIONHANDLER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <map>
#include <queue>
#include "Animation.h"
#include "AnimatedSprite.h"
#include "AnimationData.h"

using namespace sf;
using namespace std;

/**
 * Class that handles the animations.
 */
class AnimationHandler {
private:
    string path = "../../assets/";

    Texture turtleSpriteSheet;
    AnimationData *turtleAnimationData;

    Texture deathSpriteSheet;
    AnimationData *deathAnimationData;

    Texture obstacleSpriteSheet;
    AnimationData *obstacleAnimationData;

    Texture scoreSpriteSheet;
    AnimationData *scoreAnimationData;

    Texture arrowSpriteSheet;
    AnimationData *arrowAnimationData;

    map<int, pair<string, AnimatedSprite>> animations;
    map<int, Clock> timeouts;
    list<int> activeIds;
    list<int> toBeRemoved;

    queue<pair<float, float>> arrowPositions;
    queue<double> arrowRotations;

public:
    AnimationHandler();

    void update(Time deltaTime, RenderWindow *window);

    void updateLooped(Time deltaTime, int id);

    void updateTemporary(Time deltaTime, int id, RenderWindow *window);

    void updateArrow(Time deltaTime, int id, RenderWindow *window);

    void prepareArrowAnimations();

    void prepareArrowSheet();

    void prepareTurtleSheet();

    void prepareObstacleSheet();

    void prepareDeathSheet();

    void prepareScoreSheet();

    void startArrowAnimations();

    void startTurtleAnimation(int id);

    void startObstacleAnimation(int id);

    void startDeathAnimation(float x, float y);

    void startScoreAnimation(float x, float y);

    void
    startTemporaryAnimation(float x, float y, AnimationData *data, string type, Texture *spriteSheet, float rotation);

    void startLoopedAnimation(int id, AnimationData *data, string type, Texture *spriteSheet);

    AnimatedSprite &getLoopedAnimation(int id, string type);

    void remove(int id);

    void play(int id);

    void removeIfInactive(int id);
};

#endif //GAME_ANIMATIONHANDLER_H
