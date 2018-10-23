//
// Created by maxim on 13-6-18.
//

#include <iostream>
#include <sstream>
#include <ctgmath>
#include "AssetFactory.h"
#include "../storage/Variables.h"
#include "AssetLoader.h"

using namespace sf;

/**
 * LCOV:
 * These files are excluded from testing coverage because they are tested by the description-based-tests.
 * It is not a good strategy to test visuals using unit tests because verification that something is displayed
 * correctly is difficult and can be verified by a hum running the description-based-tests.
 */

/**
 * Constructor for an AssetFactory. This is used by the RenderWrapper to request pre-made SFML shapes that have had
 * preparation and are almost ready to render. The RenderWrapper only adapts positions and rotations of these shapes.
 *
 * The AssetFactory uses an AssetLoader to request textures during preparations.
 * The constructor first calls methods to prepare all needed assets.
 */
AssetFactory::AssetFactory() : assetLoader(AssetLoader()) {
    init();
}

/**
 * Init method that calls all game-component-specific preparation methods.
 */
void AssetFactory::init() {
    preparePlayerAssets();
    prepareTurtleAssets();
    prepareEggAssets();
    prepareInterfaceAssets();
    prepareObjectiveAreaAssets();
    prepareStartingAreaAssets();
    prepareBackgroundAssets();
    prepareObstacleAssets();
    prepareTrailPointAssets();
    prepareBorderAssets();
}

/**
 * Method that prepares all assets that are used to render a player.
 * - forceArea, the outer circle of the player HitBox. (only used in debug mode)
 * - impenetrableWall, the inner circle of the player HitBox. (only used in debug mode)
 */
void AssetFactory::preparePlayerAssets() {
    forceArea.setRadius(static_cast<float>(Variables::scaleToPixels(Variables::PLAYER_FORCE_RADIUS)));
    forceArea.setOutlineThickness(-5);
    forceArea.setOutlineColor(Color::Cyan);
    forceArea.setFillColor(Color::Transparent);
    forceArea.setOrigin(forceArea.getRadius(), forceArea.getRadius());

    impenetrableWall.setRadius(static_cast<float>(Variables::scaleToPixels(Variables::PLAYER_WALL_RADIUS)));
    impenetrableWall.setOutlineThickness(-3);
    impenetrableWall.setOutlineColor(Color::Cyan);
    impenetrableWall.setFillColor(Color::Transparent);
    impenetrableWall.setOrigin(impenetrableWall.getRadius(), impenetrableWall.getRadius());
}

/**
 * Method that prepares all assets that are used to render a turtle.
 * - turtle, the texture of the turtle.
 * - turtleState, the text used to draw the state of the turtle. (only used in debug mode)
 */
void AssetFactory::prepareTurtleAssets() {
    turtle.setRadius(Variables::scaleToPixels(Variables::TURTLE_RADIUS));
    turtle.setOrigin(turtle.getRadius(), turtle.getRadius());
    turtle.setTexture(assetLoader.getTurtleTexture());

    turtleState.setFont(*assetLoader.getGameFont());
    turtleState.setColor(Color::Magenta);
    turtleState.setCharacterSize(20);
}

/**
 * Method that prepares all assets that are used to render an egg.
 * - egg, the texture of the egg.
 */
void AssetFactory::prepareEggAssets() {
    egg.setRadius(Variables::scaleToPixels(Variables::TURTLE_RADIUS));
    egg.setOrigin(turtle.getRadius(), turtle.getRadius());
    egg.setTexture(assetLoader.getEggTexture());
}

/**
 * Method that prepares all interface/GUI related assets.
 * - scoreText, the text used to display the score during gameplay and once the game finished.
 * - debugText, the text used to display various statistics about the game. (only used in debug mode)
 */
void AssetFactory::prepareInterfaceAssets() {
    stringstream ss;
    ss << Variables::getScore();

    scoreText.setFont(*assetLoader.getGameFont());
    scoreText.setString("Score: " + ss.str());

    // Center the scoreText.
    FloatRect textRect = scoreText.getLocalBounds();
    scoreText.setOrigin(textRect.left + textRect.width / 2.0f,
                        textRect.top + textRect.height / 2.0f);

    debugText.setFont(*assetLoader.getGameFont());
    debugText.setColor(Color::Red);
    debugText.setString("NONE");
    debugText.setPosition(static_cast<float>(Variables::PIXEL_WIDTH / 2.0),
                          static_cast<float>(Variables::PIXEL_HEIGHT / 2.0));
}

/**
 * Method that prepares all ObjectiveArea assets.
 * - objectiveAreaCircle, the circle that represents the HitBox of the ObjectiveArea. (only used in debug mode)
 */
void AssetFactory::prepareObjectiveAreaAssets() {
    float scaledObjectiveRadius = Variables::scaleToPixels(Variables::OBJECTIVE_RADIUS);
    objectiveAreaCircle = CircleShape(scaledObjectiveRadius);
    objectiveAreaCircle.setOrigin(scaledObjectiveRadius, scaledObjectiveRadius);
    objectiveAreaCircle.setOutlineThickness(-5);
    objectiveAreaCircle.setOutlineColor(Color::Yellow);
    objectiveAreaCircle.setFillColor(Color::Transparent);
}

/**
 * Method that prepares the StartingArea assets.
 * - startingArea, the visual representation of the StartingArea that players can stand in to start the game.
 */
void AssetFactory::prepareStartingAreaAssets() {
    startingArea.setTexture(assetLoader.getStartAreaTexture());
}

/**
 * Method that prepares all background assets.
 * - background, the visual of the beach that is used during gameplay.
 * - startScreen, the visual of the starting splash screen. (deprecated, no longer used)
 * - winScreen, the visual of the winning screen when the game finished.
 * - loseScreen, the visual of the losing screen when the game finished.
 * - explainScreen, the visual of the explain screen that is shown when the game starts (GameStartState).
 * - fadeRect, a white rectangle used for fading between scenes.
 */
void AssetFactory::prepareBackgroundAssets() {
    Vector2f screenSize(static_cast<float>(Variables::PIXEL_WIDTH), static_cast<float>(Variables::PIXEL_HEIGHT));

    background.setSize(screenSize);
    background.setTexture(assetLoader.getBackgroundTexture());

    startScreen.setSize(screenSize);
    startScreen.setTexture(assetLoader.getStartScreenTexture());

    winScreen.setSize(screenSize);
    winScreen.setTexture(assetLoader.getWinScreenTexture());

    loseScreen.setSize(screenSize);
    loseScreen.setTexture(assetLoader.getLoseScreenTexture());

    explainScreen.setSize(screenSize);
    explainScreen.setTexture(assetLoader.getExplainScreenTexture());

    sponsor.setSize(screenSize);
    sponsor.setTexture(assetLoader.getSponsorTexture());

    fadeRect.setSize(screenSize);
    fadeRect.setFillColor(Color(0, 0, 0, 0));
}

/**
 * Method that prepares the Obstacle assets.
 * - obstacle, a visual representation of the obstacle.
 */
void AssetFactory::prepareObstacleAssets() {
    obstacle.setRadius(Variables::scaleToPixels(Variables::OBSTACLE_RADIUS));
    obstacle.setOrigin(obstacle.getRadius(), obstacle.getRadius());
    obstacle.setTexture(assetLoader.getObstacleTexture());
}

/**
 * Method that prepares the trail assets.
 * - trailPoint, a visual representation of dust cloud.
 * - trailPointDebug, red square used for developing the trail. (only used in debug mode)
 */
void AssetFactory::prepareTrailPointAssets() {
    trailPoint.setScale(0.2, 0.2);
    trailPointDebug.setSize(Vector2f(10, 5));
}

/**
 * Method to prepare the border assets.
 */
void AssetFactory::prepareBorderAssets() {
    RectangleShape lower;
    RectangleShape upper;
    RectangleShape left;
    RectangleShape right;

    float border_offset = Variables::scaleToPixels(Variables::TURTLE_BORDER_OFFSET);
    float objective_offset = Variables::scaleToPixels(Variables::OBJECTIVE_RADIUS);
    float turtle_offset = Variables::scaleToPixels(Variables::TURTLE_RADIUS);
    float scaling_offset = 40; // Hardcoded scaling value because this is asset specific.

    // Position each RectangleShape.
    lower.setPosition(objective_offset, Variables::PIXEL_HEIGHT - border_offset + turtle_offset);
    lower.setSize(Vector2f(Variables::PIXEL_WIDTH - 2 * objective_offset,
                           border_offset + turtle_offset + scaling_offset));
    upper.setPosition(objective_offset, -scaling_offset);
    upper.setSize(Vector2f(Variables::PIXEL_WIDTH - 2 * objective_offset,
                           border_offset - turtle_offset + scaling_offset));

    left.setPosition(-scaling_offset, objective_offset);
    left.setSize(Vector2f(border_offset - turtle_offset + scaling_offset,
                          Variables::PIXEL_HEIGHT - 2 * objective_offset));
    right.setPosition(Variables::PIXEL_WIDTH - border_offset + turtle_offset, objective_offset);
    right.setSize(Vector2f(border_offset - turtle_offset + scaling_offset,
                           Variables::PIXEL_HEIGHT - 2 * objective_offset));

    //Set the colours.
    lower.setTexture(assetLoader.getLongWallTexture());
    upper.setTexture(assetLoader.getLongWallTexture());
    left.setTexture(assetLoader.getShortWallTexture());
    right.setTexture(assetLoader.getShortWallTexture());

    // Add the shapes to the list of borders.
    borders.push_back(lower);
    borders.push_back(upper);
    borders.push_back(left);
    borders.push_back(right);
}

// GETTERS

/**
 * Getter for the prepared CircleShape for the forceArea.
 * @return forceArea - a CircleShape.
 */
const CircleShape &AssetFactory::getForceArea() const {
    return forceArea;
}

/**
 * Getter for the prepared CircleShape for the impenetrableWall.
 * @return impenetrableWall - a CircleShape.
 */
const CircleShape &AssetFactory::getImpenetrableWall() const {
    return impenetrableWall;
}

/**
 * Getter for the prepared CircleShape for the turtle.
 * @return turtle - a CircleShape.
 */
const CircleShape &AssetFactory::getTurtle() const {
    return turtle;
}

/**
 * Getter for the prepared Text for the turtleState.
 * @return turtleState - a Text object.
 */
const Text &AssetFactory::getTurtleState() const {
    return turtleState;
}

/**
 * Getter for the prepared CircleShape for the egg.
 * @return egg - a CircleShape.
 */
const CircleShape &AssetFactory::getEgg() const {
    return egg;
}

/**
 * Getter for the prepared CircleShape for the forceArea.
 * @return forceArea - a CircleShape.
 */
const CircleShape &AssetFactory::getObstacle() const {
    return obstacle;
}

/**
 * Getter for the prepared Text for the debugText.
 * @return debugText - a Text object.
 */
const Text &AssetFactory::getDebugText() const {
    return debugText;
}

/**
 * Getter for the prepared Text for the scoreText.
 * @return scoreText - a Text object.
 */
const Text &AssetFactory::getScoreText() const {
    return scoreText;
}

/**
 * Getter for the prepared CircleShape for the objectiveAreaCircle
 * @return objectiveAreaCircle - a CircleShape.
 */
const CircleShape &AssetFactory::getObjectiveAreaCircle() const {
    return objectiveAreaCircle;
}

/**
 * Getter for the prepared RectangleShape for the startingArea.
 * @return startingArea - a RectangleShape that is the size of the screen.
 */
const RectangleShape &AssetFactory::getStartingArea() const {
    return startingArea;
}

/**
 * Getter for the prepared RectangleShape for the background.
 * @return background - a RectangleShape that is the size of the screen.
 */
const RectangleShape &AssetFactory::getBackground() const {
    return background;
}

/**
 * Getter for the prepared RectangleShape for the startScreen.
 * @return startScreen - a RectangleShape that is the size of the screen.
 */
const RectangleShape &AssetFactory::getStartScreen() const {
    return startScreen;
}

/**
 * Getter for the prepared RectangleShape for the winScreen.
 * @return winScreen - a RectangleShape that is the size of the screen.
 */
const RectangleShape &AssetFactory::getWinScreen() const {
    return winScreen;
}

/**
 * Getter for the prepared RectangleShape for the loseScreen.
 * @return loseScreen - a RectangleShape that is the size of the screen.
 */
const RectangleShape &AssetFactory::getLoseScreen() const {
    return loseScreen;
}

/**
 * Getter for the prepared RectangleShape for the explainScreen.
 * @return explainScreen - a RectangleShape that is the size of the screen.
 */
const RectangleShape &AssetFactory::getExplainScreen() const {
    return explainScreen;
}

/**
 * Getter for the prepared RectangleShape for the trailPointDebug object.
 * @return trailPointDebug - a RectangleShape.
 */
const RectangleShape &AssetFactory::getTrailPointDebug() const {
    return trailPointDebug;
}

/**
 * Getter for a specific a trailPoint object with an id corresponding to a specific specific trailPoint texture.
 * @param id - the id of the texture that should be used.
 * @return trailPoint - with the correct texture set.
 */
const Sprite &AssetFactory::getTrailPoint(int id) {
    trailPoint.setTexture(*assetLoader.getTrailTexture(id));
    return trailPoint;
}

/**
 * Return the asset factory game borders.
 * @return borders - The game borders.
 */
const list<RectangleShape> &AssetFactory::getBorders() const {
    return borders;
}

/**
 * Getter for the sponsor.
 * @return sponsor - The sponsor.
 */
const RectangleShape &AssetFactory::getSponsor() const {
    return sponsor;
}