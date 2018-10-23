//
// Created by maxim on 13-6-18.
//

#ifndef GAME_ASSETFACTORY_H
#define GAME_ASSETFACTORY_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include "AssetLoader.h"
#include "AnimationHandler.h"

using namespace sf;
using namespace std;

/**
 * AssetFactory object, preparing SFML objects is done here.
 * These objects are adapted during runtime by the RenderWrapper to suit
 * specific state-dependent needs.
 */
class AssetFactory {

private:
    AssetLoader assetLoader;

    // Player
    CircleShape forceArea;
    CircleShape impenetrableWall;

    // Turtle
    CircleShape turtle;
    Text turtleState;

    // Egg
    CircleShape egg;

    // Obstacle
    CircleShape obstacle;

    // Interface
    Text scoreText;
    Text debugText;

    // Objective Areas
    CircleShape objectiveAreaCircle;

    // Starting Area
    RectangleShape startingArea;

    // Backgrounds
    RectangleShape startScreen;
    RectangleShape winScreen;
    RectangleShape loseScreen;
    RectangleShape background;
    RectangleShape explainScreen;
    RectangleShape sponsor;

    // Trail
    RectangleShape trailPointDebug;
    Sprite trailPoint;

    // Border
    std::list<RectangleShape> borders;

    // Rectangle and variables used for screen fading.
    RectangleShape fadeRect;

    void preparePlayerAssets();

    void prepareTurtleAssets();

    void prepareEggAssets();

    void prepareInterfaceAssets();

    void prepareObjectiveAreaAssets();

    void prepareStartingAreaAssets();

    void prepareBackgroundAssets();

    void prepareObstacleAssets();

    void prepareTrailPointAssets();

    void prepareBorderAssets();

public:
    void init();

    AssetFactory();

    const CircleShape &getForceArea() const;

    const CircleShape &getImpenetrableWall() const;

    const CircleShape &getTurtle() const;

    const CircleShape &getEgg() const;

    const Text &getTurtleState() const;

    const CircleShape &getObstacle() const;

    const Text &getDebugText() const;

    const Text &getScoreText() const;

    const CircleShape &getObjectiveAreaCircle() const;

    const RectangleShape &getStartingArea() const;

    const RectangleShape &getBackground() const;

    const RectangleShape &getStartScreen() const;

    const RectangleShape &getWinScreen() const;

    const RectangleShape &getLoseScreen() const;

    const RectangleShape &getExplainScreen() const;

    const RectangleShape &getSponsor() const;

    const RectangleShape &getTrailPointDebug() const;

    const Sprite &getTrailPoint(int id);

    const list <RectangleShape> &getBorders() const;

};

#endif //GAME_ASSETFACTORY_H