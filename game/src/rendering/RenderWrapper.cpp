//
// Created by maxim on 17-5-18.
//

#include <iostream>
#include "RenderWrapper.h"
#include "../storage/Variables.h"
#include "../gameEntities/turtle/turtleStates/TurtleSpawnState.h"
#include "../utility/MathUtilities.h"
#include "../gameEntities/TrailPoint.h"
#include "../gameEntities/ObjectiveArea.h"
#include <sstream>
#include <cmath>

using namespace std;
using namespace sf;

/**
 * LCOV:
 * These files are excluded from testing coverage because they are tested by the description-based-tests.
 * It is not a good strategy to test visuals using unit tests because verification that something is displayed
 * correctly is difficult and can be verified by a hum running the description-based-tests.
 */

/**
 * Constructor of a RenderWrapper. It still needs the RenderWindow that it is wrapping,so make sure to call setWindow
 * when using this constructor.
 * Only use this constructor if you want to load assets before using the window.
 * The constructor creates a AssetFactory which prepares sprites, textures and shapes for each entity that has
 * to be rendered in the game. Each render method requests the needed assets from the AssetFactory.
 */
RenderWrapper::RenderWrapper() : assetFactory(AssetFactory()) {
    this->renderWindow = NULL;
    this->init();
}

/**
 * Overloaded constructor for renderWindow.
 * @param renderWindow - The active RenderWindow.
 */
RenderWrapper::RenderWrapper(RenderWindow *renderWindow) : assetFactory(AssetFactory()) {
    this->renderWindow = renderWindow;
    this->init();
}

/**
 * Initializes the renderWrapper and loads all assets of assetFactory.
 */
void RenderWrapper::init() {
    // Debug Specific
    debugTimer = 0.0;
    frameCount = 0;
    displayedFrameCount = -1;
    displayedFPS = -1;

    turtleShape = assetFactory.getTurtle();
    resetFadeVariables();
    assetFactory.init();
}

/**
 * Setter for the renderWindow.
 * @param renderWindow - The window of rendering to be set.
 */
void RenderWrapper::setWindow(RenderWindow *renderWindow) {
    this->renderWindow = renderWindow;
}

/**
 * Setter for the frame time.
 * Indicates the time that passed during this frame in seconds.
 * @param frameTime - Time passed in seconds.
 */
void RenderWrapper::setFrameTime(float frameTime) {
    RenderWrapper::frameTime = frameTime;
}

/**
 * Updates the animations in the asset factory.
 * @param deltaTime - The frame time in an SF::Time object.
 */
void RenderWrapper::updateAnimations(Time deltaTime, RenderWindow *window) {
    animationHandler.update(deltaTime, window);
}

/**
 * Starts a death animation in the animation handler at a certain position.
 * @param x - The x coordinate of the animation.
 * @param y - The y coordinate of the animation.
 */
void RenderWrapper::startDeathAnimation(float x, float y) {
    animationHandler.startDeathAnimation(x, y);
}

/**
 * Starts a score animation in the animation handler at a certain position.
 * @param x - The x coordinate of the animation.
 * @param y - The y coordinate of the animation.
 */
void RenderWrapper::startScoreAnimation(float x, float y) {
    animationHandler.startScoreAnimation(x, y);
}

/**
 * Starts the first 4 arrow animations, each pointing to a sea.
 */
void RenderWrapper::startArrowAnimations() {
    animationHandler.startArrowAnimations();
}

/**
 * Getter for the fadeAlpha value.
 * This is used to indicate whether the gamestate can already change or not.
 * @return fadeAlpha - the alpha value.
 */
double RenderWrapper::getFadeAlpha() {
    return fadeAlpha;
}

/**
 * Called before all rendering starts, old visuals are removed from the frame.
 */
void RenderWrapper::clearFrame() {
    renderWindow->clear();
}

/**
 * Called at the end of rendering, all visuals are pushed towards the screen, displaying them all at once.
 */
void RenderWrapper::displayFrame() {
    renderWindow->display();
}

/**
 * Method for positioning and rotating a CircleShape.
 * This is often needed before rendering game entities, thus it was put into a private method.
 * @param target - The CircleShape that is to be positioned and rotated.
 * @param x - The x coordinate of the entity.
 * @param y - The y coordinate of the entity.
 * @param direction - The direction that the entity is facing.
 */
void RenderWrapper::positionAndRotateShape(CircleShape *target, double x, double y, double direction) {
    target->setPosition(static_cast<float>(x), static_cast<float>(y));
    target->setRotation(static_cast<float>(direction));
}

/**
 * Method for positioning and rotating an AnimatedSprite.
 * This is often needed before rendering game entities, thus it was put into a private method.
 * @param target - The AnimatedSprite that is to be positioned and rotated.
 * @param x - The x coordinate of the entity.
 * @param y - The y coordinate of the entity.
 * @param direction - The direction that the entity is facing.
 */
void RenderWrapper::positionAndRotateShape(AnimatedSprite *target, double x, double y, double direction) {
    target->setPosition(static_cast<float>(x), static_cast<float>(y));
    target->setRotation(static_cast<float>(direction));
}

/**
 * Method for drawing a line, this is used for debugging.
 * @param startX - The x coordinate where the line should start.
 * @param startY - The y coordinate where the line should start.
 * @param direction - The direction that the line should go to.
 * @param length - The length of the line.
 */
void RenderWrapper::drawLine(double startX, double startY, double direction, float length) {
    // Construct a vector of the start position and a direction vector.
    sf::Vector2f posVector = sf::Vector2f(static_cast<float>(startX), static_cast<float>(startY));
    sf::Vector2f dirVector = sf::Vector2f(static_cast<float>(cos(direction)), static_cast<float>(sin(direction)));

    // Create a two point vertex using the direction vector and the length.
    sf::Vertex line[] = {
            sf::Vertex(posVector),
            sf::Vertex(posVector + (dirVector * length))
    };

    // Set the color and draw the vertex as a line.
    line[0].color = Color::Red;
    line[1].color = Color::Red;
    renderWindow->draw(line, 2, sf::Lines);
}

/**
 * Method that renders a player entity.
 * @param x - The x coordinate of the player within the game window.
 * @param y - The y coordinate of the player within the game window.
 * @param forceColliding - A boolean used for displaying collision debug draws.
 * @param timeout - Timer value used for showing the timeout value of the player
 */
void RenderWrapper::renderPlayer(double x, double y, bool forceColliding, double timeout) {
    // Only draw players in debug mode.
    if (!Variables::DEBUG_MODE) {
        return;
    }

    // Get the needed assets from the AssetFactory.
    CircleShape forceArea = assetFactory.getForceArea();
    CircleShape impenetrableWall = assetFactory.getImpenetrableWall();

    // Adapt colours for debugging.
    if (forceColliding) {
        forceArea.setOutlineColor(Color::Red);
    } else {
        impenetrableWall.setOutlineColor(Color::Cyan);
    }

    // Set the correct positions of the shapes.
    forceArea.setPosition(static_cast<float>(x), static_cast<float>(y));
    impenetrableWall.setPosition(static_cast<float>(x), static_cast<float>(y));

    // Draw the shapes onto the current frame.
    renderWindow->draw(forceArea);
    renderWindow->draw(impenetrableWall);

    // If debug mode is active draw the timeout of the player on the screen.
    Text turtleState = assetFactory.getTurtleState();
    stringstream stateString;
    stateString << timeout;
    turtleState.setString(stateString.str());
    turtleState.setPosition(static_cast<float>(x), static_cast<float>(y - 50.0));
    // Draw the debug text onto the current frame.
    renderWindow->draw(turtleState);
}

/**
 * Method that renders a turtle entity.
 * @param x - The x coordinate of the turtle within the game window.
 * @param y - The x coordinate of the turtle within the game window.
 * @param direction - The direction that the turtle is facing.
 * @param alpha - Alpha value between 0 and 255 used to fade in turtles.
 * @param stateIdentifier - The stateIdentifier of the current state of the turtle, used for debug draws.
 * @param id - The id of the turtle. This is used by the animation handler to keep track.
 */
void
RenderWrapper::renderTurtle(double x, double y, double direction, double alpha, std::string *stateIdentifier, int id) {
    AnimatedSprite turtle = animationHandler.getLoopedAnimation(id, "turtle");

    // Adjust turtle transparency according to alpha value.
    if (alpha > 255) {
        alpha = 255;
    }
    Color turtleColor(turtleShape.getFillColor().r, turtleShape.getFillColor().g, turtleShape.getFillColor().b,
                      static_cast<Uint8>(alpha));
    turtle.setColor(turtleColor);

    // Set position and rotation of the turtle. (Convert radians to degrees)
    positionAndRotateShape(&turtle, x, y, (direction * 180.0 / M_PI) + 90.0);

    // Draw the shape onto the current frame.
    renderWindow->draw(turtle);

    if (Variables::DEBUG_MODE) {

        // If debug mode is active draw the state of the turtle on the screen.
        Text turtleState = assetFactory.getTurtleState();
        stringstream stateString;
        stateString << *stateIdentifier;
        turtleState.setString(stateString.str());
        turtleState.setPosition(static_cast<float>(x), static_cast<float>(y));
        // Draw the debug text onto the current frame.
        renderWindow->draw(turtleState);

        // Also draw a line indicating the direction of the turtle.
        drawLine(x, y, direction, 350.f);
    }
}

/**
 * Method that renders an egg. This is the representation of a Turtle entity that is in the TurtleSpawnState.
 * @param x - The x coordinate of the egg within the game window.
 * @param y - The y coordinate of the egg within the game window.
 * @param direction - The direction that the egg is facing.
 * @param alpha - The transparency value of the egg, used for fading the egg in as it spawns.
 */
void RenderWrapper::renderEgg(double x, double y, double direction, double alpha) {
    CircleShape egg = assetFactory.getEgg();

    // Adjust egg transparency according to alpha value.
    if (alpha > 255) {
        alpha = 255;
    }
    Color eggColor(egg.getFillColor().r, egg.getFillColor().g, egg.getFillColor().b, static_cast<Uint8>(alpha));
    egg.setFillColor(eggColor);

    // Set position and rotation of the egg. (Convert radians to degrees)
    positionAndRotateShape(&egg, x, y, (direction * 180.0 / M_PI) + 90.0);

    // Draw the shape onto the current frame.
    renderWindow->draw(egg);
}

/**
 * Renders the trail of a player onto the scene.
 * @param points - All the points in the trail that must be drawn.
 */
void RenderWrapper::renderTrail(std::list<TrailPoint *> points) {
    // Render each trailPoint either as debug point or with the actual visualization.
    for (TrailPoint *point : points) {
        if (Variables::DEBUG_MODE) {
            RectangleShape trailPointDebug = assetFactory.getTrailPointDebug();
            trailPointDebug.setPosition(static_cast<float>(point->getX()), static_cast<float>(point->getY()));
            trailPointDebug.setRotation(point->getDirection());
            trailPointDebug.setFillColor(Color(255, 0, 0, static_cast<Uint8>(255 * point->getOpacity())));
            renderWindow->draw(trailPointDebug);
        } else {
            Sprite trailPoint = assetFactory.getTrailPoint(point->getTextureId());
            trailPoint.setPosition(static_cast<float>(point->getX()), static_cast<float>(point->getY()));
            trailPoint.setRotation(point->getDirection());
            trailPoint.setColor(Color(255, 255, 255, static_cast<Uint8>(255 * point->getOpacity())));
            trailPoint.setScale(0.2, 0.2);
            renderWindow->draw(trailPoint);
        }
    }
}

/**
 * Renders an obstacle.
 * @param x - X coordinate of the obstacle.
 * @param y - Y coordinate of the obstacle.
 * @param rotation - The rotation at which the obstacle should be placed.
 * @param alpha - The fading value of the obstacle.
 */
void RenderWrapper::renderObstacle(double x, double y, double rotation, double alpha, int id) {
    CircleShape obstacleShape = assetFactory.getObstacle();
    AnimatedSprite obstacle = animationHandler.getLoopedAnimation(id, "obstacle");
    positionAndRotateShape(&obstacle, x, y, rotation);

    // Adjust obstacle transparency according to alpha value.
    if (alpha > 255) {
        alpha = 255;
    } else if (alpha < 0) {
        alpha = 0;
    }
    Color obstacleColor(obstacleShape.getFillColor().r, obstacleShape.getFillColor().g, obstacleShape.getFillColor().b,
                        static_cast<Uint8>(alpha));
    obstacle.setColor(obstacleColor);

    renderWindow->draw(obstacle);
}

/**
 * Method that renders debug info in the middle of the screen.
 * Only updates at an interval of DEBUG_UPDATE_RATE.
 * This currently contains:
 * - Amount of frames that were processed.
 * - Current estimation of the frames per second.
 * - The current state of the game.
 * - The number of players.
 * - The number of turtles.
 * @param inputData - The InputData of the current frame, used to retrieve the frameTime.
 */
void RenderWrapper::renderDebugInfo(InputData *inputData) {
    // Decrement the debug update timer.
    debugTimer -= inputData->getFrameTime();
    frameCount++;

    // Variables that should only be updated visually once in a while for readability.
    if (debugTimer <= 0.0) {
        displayedFrameCount = frameCount;
        displayedFPS = ((int) (1.0 / inputData->getFrameTime()));
        debugTimer = Variables::DEBUG_UPDATE_RATE;
    }

    Text debugText = assetFactory.getDebugText();

    // Update the debug string.
    stringstream stat;
    stat << "FrameCount = " << displayedFrameCount <<
         "\nFPS = " << displayedFPS <<
         "\nGameState = " << Variables::DEBUG_GAME_STATE <<
         "\nPlayerCount = " << Variables::DEBUG_PLAYER_COUNT <<
         "\nTurtleCount = " << Variables::DEBUG_TURTLE_COUNT <<
         "\nscore = " << Variables::getScore() <<
         "\nTURTLES_LEFT_TO_SPAWN = " << Variables::DEBUG_TURTLES_LEFT_TO_SPAWN <<
         "\nDEAD_TURTLES = " << Variables::DEAD_TURTLES;
    debugText.setString(stat.str());

    // Draw the text to the screen.
    renderWindow->draw(debugText);
}

/**
 * Method that renders the GUI based on the current score in the Variables class.
 */
void RenderWrapper::renderGUI() {
    double x = (Variables::PIXEL_WIDTH / 2.0f) + 4;
    double y = 54;
    drawScoreText(x, y, Variables::GUI_SCORE_GAME);
}

/**
 * Method that renders the hitBoxes of the objective area.
 * @param x - x Location of the objective area.
 * @param y - y Location of the objective area.
 * @param radius - Radius of the objective area.
 * @param swimDirection - Direction turtles swim in when in this objective area.
 */
void RenderWrapper::renderObjectiveArea(double x, double y, double radius, double swimDirection) {
    // Only draw objective area circles in DEBUG_MODE.
    if (!Variables::DEBUG_MODE) {
        return;
    }

    CircleShape objectiveAreaCircle = assetFactory.getObjectiveAreaCircle();

    objectiveAreaCircle.setOrigin(static_cast<float>(radius), static_cast<float>(radius));
    objectiveAreaCircle.setPosition(static_cast<float>(x), static_cast<float>(y));
    objectiveAreaCircle.setRadius(static_cast<float>(radius));

    // Draw a line that shows the direction that Turtles should swim to.
    drawLine(x, y, swimDirection, (float) Variables::VISIBILITY_MARGIN);

    renderWindow->draw(objectiveAreaCircle);
}

/**
 * Function to render the game borders.
 */
void RenderWrapper::renderBorders() {
    for (RectangleShape border : assetFactory.getBorders()) {
        renderWindow->draw(border);
    }
}

/**
 * Method that renders a starting area with specific coordinates.
 * @param xMin - The x coordinate of the left border.
 * @param xMax - The x coordinate of the right border.
 * @param yMin - The y coordinate of the lower border.
 * @param yMax - the y coordinate of the upper border.
 */
void RenderWrapper::renderStartingArea(double xMin, double xMax, double yMin, double yMax) {
    RectangleShape startingArea = assetFactory.getStartingArea();
    startingArea.setSize(Vector2f(static_cast<float>(xMax - xMin), static_cast<float>(yMax - yMin)));
    startingArea.setPosition(static_cast<float>(xMin), static_cast<float>(yMin));
    renderWindow->draw(startingArea);
}

/**
 * Method that renders the background.
 */
void RenderWrapper::renderBackground() {
    renderWindow->draw(assetFactory.getBackground());
}

/**
 * Method that renders the startScreen.
 */
void RenderWrapper::renderStartScreen() {
    renderWindow->draw(assetFactory.getStartScreen());
}

/**
 * Method that renders the winScreen including the final score.
 */
void RenderWrapper::renderWinScreen() {
    renderWindow->draw(assetFactory.getWinScreen());

    double x = Variables::PIXEL_WIDTH * 0.1f;
    double y = Variables::PIXEL_HEIGHT * 0.7f;
    drawFinalScoreText(x, y, Variables::GUI_SCORE_FINISH, true);
}

/**
 * Method that renders the loseScreen including the final score.
 */
void RenderWrapper::renderLoseScreen() {
    renderWindow->draw(assetFactory.getLoseScreen());

    double x = Variables::PIXEL_WIDTH * 0.1f;
    double y = Variables::PIXEL_HEIGHT * 0.7f;
    drawFinalScoreText(x, y, Variables::GUI_SCORE_FINISH, false);

}

/**
 * Method that renders the explainScreen.
 */
void RenderWrapper::renderExplainScreen() {
    renderWindow->draw(assetFactory.getExplainScreen());
}

/**
 * Method for rendering the scoreText with a specific size and position.
 * This is used during the GameRunningState.
 * @param x - The x position of the scoreText.
 * @param y - The y position of the scoreText.
 * @param size - The size of the characters in the scoreText.
 */
void RenderWrapper::drawScoreText(double x, double y, double size) {
    Text scoreText = assetFactory.getScoreText();

    // Update the text to display the correct score.
    stringstream ss;
    ss << Variables::getScore();
    scoreText.setString("Score: " + ss.str());

    // Set the size.
    scoreText.setCharacterSize(static_cast<unsigned int>(ceil(Variables::scaleToPixels(size))));

    // Draw the text in black as a shadow to give the display more depth.
    scoreText.setColor(Color(0, 0, 0));
    scoreText.setPosition(static_cast<float>(x - 76.0), static_cast<float>(y));
    renderWindow->draw(scoreText);

    // Draw the text with an offset in white on top of the shadow.
    scoreText.setColor(Color(0, 255, 0));
    scoreText.setPosition(static_cast<float>(x - 80.0), static_cast<float>(y - 4.0));
    renderWindow->draw(scoreText);
}

/**
 * Method for rendering the final scoreText with a specific size and position.
 * This is used by both the finishing screens.
 * @param x - The x position of the scoreText.
 * @param y - The y position of the scoreText.
 * @param size - The size of the characters in the scoreText.
 * @param win - Boolean if the player won or lost.
 */
void RenderWrapper::drawFinalScoreText(double x, double y, double size, bool win) {
    std::string message1;
    std::string message2;
    if (win) {
        message1 = "Thanks for playing!\nYou saved a total of ";
        message2 = " turtles.\nYou are amazing!";
    } else {
        message1 = "Thanks for playing!\nYou saved a total of ";
        message2 = " turtles.\nHowever, you killed ";
    }

    Text scoreText = assetFactory.getScoreText();

    // Update the text to display the correct score.
    stringstream ss1;
    ss1 << Variables::getScore();
    if (win) {
        scoreText.setString(message1 + ss1.str() + message2);
    } else {
        stringstream ss2;
        ss2 << Variables::DEAD_TURTLES;
        scoreText.setString(message1 + ss1.str() + message2 + ss2.str() + ".");
    }

    // Set the size.
    scoreText.setCharacterSize(static_cast<unsigned int>(ceil(Variables::scaleToPixels(size))));

    if (Variables::SHOW_SPONSOR) {
        RectangleShape sponsor = assetFactory.getSponsor();
        sponsor.setScale(0.3, 0.3);
        sponsor.setPosition(static_cast<float>((Variables::PIXEL_WIDTH - sponsor.getSize().x * 0.3) / 2),
                            static_cast<float>((Variables::PIXEL_HEIGHT - sponsor.getSize().y * 0.3) / 2));
        renderWindow->draw(sponsor);
    }

    // Draw the text in black as a shadow to give the display more depth.
    scoreText.setColor(Color(0, 0, 0));
    scoreText.setPosition(static_cast<float>(x), static_cast<float>(y));
    renderWindow->draw(scoreText);

    // Draw the text with an offset in white on top of the shadow.
    scoreText.setColor(Color(255, 102, 0));
    scoreText.setPosition(static_cast<float>(x - 4.0), static_cast<float>(y - 4.0));
    renderWindow->draw(scoreText);

}

/**
 * Method for fading out of the running state.
 */
void RenderWrapper::fadeOut() {
    if (fadeOutTimer < Variables::SCREEN_FADE_TIME) {
        fadeAlpha = (fadeOutTimer / Variables::SCREEN_FADE_TIME) * 255.0;
        fadeOutTimer += frameTime;
    } else {
        fadeAlpha = 255.0;
    }
    fadeRect.setFillColor(Color(255, 255, 255, static_cast<Uint8>(fadeAlpha)));
    renderWindow->draw(fadeRect);
}

/**
 * Method for fading into the finishing state.
 */
void RenderWrapper::fadeIn() {
    if (fadeInTimer < Variables::SCREEN_FADE_TIME) {
        fadeAlpha = 255.0 - (fadeInTimer / Variables::SCREEN_FADE_TIME) * 255.0;
        fadeInTimer += frameTime;
    } else {
        fadeAlpha = 0.0;
    }
    fadeRect.setFillColor(Color(255, 255, 255, static_cast<Uint8>(fadeAlpha)));
    renderWindow->draw(fadeRect);
}

/**
 * Resets all the variables used in the fading so the game can be restarted.
 */
void RenderWrapper::resetFadeVariables() {
    fadeOutTimer = 0.0;
    fadeInTimer = 0.0;
    fadeAlpha = 0.0;
    frameTime = 0.0f;
}