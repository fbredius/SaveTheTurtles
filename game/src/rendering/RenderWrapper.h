//
// Created by maxim on 17-5-18.
//

#ifndef GAME_RENDERWRAPPER_H
#define GAME_RENDERWRAPPER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include "../storage/HitBox.h"
#include "../storage/InputData.h"
#include "../gameEntities/TrailPoint.h"
#include "AssetFactory.h"

using namespace sf;
using namespace std;

/**
 * The renderWrapper object, the object that handles all rendering of the game.
 * It is passed to all the GameEntities and it allows to separate the game
 * logic from SFML because all the SFML code is contained in this wrapper.
 */
class RenderWrapper {
private:
    RenderWindow *renderWindow;
    AnimationHandler animationHandler;
    AssetFactory assetFactory;

    // DEBUG
    double debugTimer;
    int frameCount;
    int displayedFrameCount;
    double displayedFPS;

    // Rectangle and variables used for screen fading.
    RectangleShape fadeRect;
    double fadeOutTimer;
    double fadeInTimer;
    double fadeAlpha;

    // Turtle animations
    CircleShape turtleShape;

    // Frame time to be used in scene transitions.
    float frameTime;

    void positionAndRotateShape(CircleShape *target, double x, double y, double direction);

    void positionAndRotateShape(AnimatedSprite *target, double x, double y, double direction);

    void drawLine(double startX, double startY, double direction, float length);

    void drawScoreText(double x, double y, double size);

    void drawFinalScoreText(double x, double y, double size, bool win);

public:
    explicit RenderWrapper();

    explicit RenderWrapper(RenderWindow *renderWindow);

    void init();

    void setWindow(RenderWindow *renderWindow);

    void setFrameTime(float frameTime);

    double getFadeAlpha();

    void clearFrame();

    void displayFrame();

    void renderPlayer(double x, double y, bool forceColliding, double timeout);

    void renderTurtle(double x, double y, double direction, double alpha, std::string *stateIdentifier, int id);

    void renderEgg(double x, double y, double direction, double alpha);

    void renderTrail(std::list<TrailPoint *> points);

    void renderObstacle(double x, double y, double rotation, double alpha, int id);

    void renderDebugInfo(InputData *inputData);

    void renderGUI();

    void renderObjectiveArea(double x, double y, double radius, double swimDirection);

    void renderBorders();

    void renderStartingArea(double xMin, double xMax, double yMin, double yMax);

    void renderBackground();

    void renderStartScreen();

    void renderWinScreen();

    void renderLoseScreen();

    void renderExplainScreen();

    void fadeOut();

    void fadeIn();

    void resetFadeVariables();

    void updateAnimations(Time deltaTime, RenderWindow *window);

    void startDeathAnimation(float x, float y);

    void startScoreAnimation(float x, float y);

    void startArrowAnimations();

};

#endif //GAME_RENDERWRAPPER_H