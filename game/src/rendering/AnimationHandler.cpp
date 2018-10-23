//
// Created by timbuntu on 8-6-18.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <cmath>
#include "AnimationHandler.h"
#include "../storage/Variables.h"

/**
 * Constructor for the Animation Handler class.
 */
AnimationHandler::AnimationHandler() {
    if (Variables::IMOVE_ACTIVE) {
        path = "../game/assets/";
    }
    prepareTurtleSheet();
    prepareObstacleSheet();
    prepareDeathSheet();
    prepareScoreSheet();
    prepareArrowSheet();
}

/**
 * Loads the turtle sheet, and prepares the animation data for the turtles.
 */
void AnimationHandler::prepareTurtleSheet() {
    // Load turtle spritesheet.
    if (!AnimationHandler::turtleSpriteSheet.loadFromFile(path + "spritesheets/turtlesheet.png")) {
        std::cout << "Error loading turtle sprite sheet" << std::endl;
    }
    int width = 256;
    int height = 256;
    AnimationData *animationData = new AnimationData(Variables::TURTLE_ANIMATION_SPEED, width, height,
                                                     Variables::TURTLE_RADIUS);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            animationData->addFrame(width * j, height * i);
        }
    }

    turtleAnimationData = animationData;
}

/**
 * Loads the obstacle sheet, and prepares the animation data for the obstacles.
 */
void AnimationHandler::prepareObstacleSheet() {
    // Load obstacle spritesheet.
    if (!AnimationHandler::obstacleSpriteSheet.loadFromFile(path + "spritesheets/obstaclesheet.png")) {
        std::cout << "Error loading obstacle sprite sheet" << std::endl;
    }
    int width = 600;
    int height = 600;
    AnimationData *animationData = new AnimationData(Variables::OBSTACLE_ANIMATION_SPEED, width, height,
                                                     Variables::OBSTACLE_RADIUS);
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 3; j++) {
            animationData->addFrame(width * i, height * j);
        }
    }
    animationData->addFrame(7800, 0);
    obstacleAnimationData = animationData;
}

/**
 * Loads the death sheet, and prepares the animation data for the death animations.
 */
void AnimationHandler::prepareDeathSheet() {
    // Load death spritesheet.
    if (!AnimationHandler::deathSpriteSheet.loadFromFile(path + "spritesheets/deathsheet.png")) {
        std::cout << "Error loading death sprite sheet" << std::endl;
    }
    // Prepare the animation data.
    int width = 600;
    int height = 600;
    AnimationData *animationData = new AnimationData(Variables::DEATH_ANIMATION_SPEED, width, height,
                                                     Variables::DEATH_ANIMATION_SIZE);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 5; j++) {
            animationData->addFrame(width * j, height * i);
        }
    }
    deathAnimationData = animationData;
}

/**
 * Loads the score sheet, and prepares the animation data for the score animations.
 */
void AnimationHandler::prepareScoreSheet() {
    // Load score spritesheet.
    if (!AnimationHandler::scoreSpriteSheet.loadFromFile(path + "spritesheets/scoresheet.png")) {
        std::cout << "Error loading score sprite sheet" << std::endl;
    }
    int width = 600;
    int height = 600;
    AnimationData *animationData = new AnimationData(Variables::SCORE_ANIMATION_SPEED, width, height,
                                                     Variables::SCORE_ANIMATION_SIZE);
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 3; j++) {
            animationData->addFrame(width * i, height * j);
        }
    }
    scoreAnimationData = animationData;
}

/**
 * Prepares the arrow sheet and animation data.
 */
void AnimationHandler::prepareArrowSheet() {
    // Load arrow spritesheet.
    if (!AnimationHandler::arrowSpriteSheet.loadFromFile(path + "spritesheets/arrowsheet.png")) {
        std::cout << "Error loading arrow sprite sheet" << std::endl;
    }
    int width = 650;
    int height = 1300;
    AnimationData *animationData = new AnimationData(Variables::ARROW_ANIMATION_SPEED, width, height,
                                                     Variables::ARROW_ANIMATION_SIZE);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 6; j++) {
            animationData->addFrame(width * j, height * i);
        }
    }
    arrowAnimationData = animationData;
}

/**
 * Prepares the list of locations and rotations for the arrows.
 */
void AnimationHandler::prepareArrowAnimations() {
    // Add more offsets if you want more arrows.
    // You also need the range of the bottom for-loop to add the corresponding rotations.
    list<float> offsets;
    offsets.push_back(0.24f);
    offsets.push_back(0.38f);
    offsets.push_back(0.58f);

    float widthMiddle = Variables::PIXEL_WIDTH / 2;
    float heightMiddle = Variables::PIXEL_HEIGHT / 2;

    // Increase amount of iterations if you want more iterations of arrows.
    for (int j = 0; j < 3; j++) {
        for (float offset : offsets) {
            arrowPositions.push(
                    pair<float, float>(widthMiddle - (offset * widthMiddle), heightMiddle - (offset * heightMiddle)));
            arrowPositions.push(
                    pair<float, float>(widthMiddle - (offset * widthMiddle), heightMiddle + (offset * heightMiddle)));
            arrowPositions.push(
                    pair<float, float>(widthMiddle + (offset * widthMiddle), heightMiddle - (offset * heightMiddle)));
            arrowPositions.push(
                    pair<float, float>(widthMiddle + (offset * widthMiddle), heightMiddle + (offset * heightMiddle)));

        }

        // Add the rotations for the arrows.
        for (int i = 0; i < 3; i++) {
            arrowRotations.push(315.f - 10.f);
            arrowRotations.push(225.f + 10.f);
            arrowRotations.push(45.f + 10.f);
            arrowRotations.push(135.f - 10.f);
        }
    }
}

/**
 * Updates every animation in the animations map.
 * @param deltaTime - The amount of time that has passed since last update.
 */
void AnimationHandler::update(Time deltaTime, RenderWindow *window) {
    for (int i : activeIds) {
        string type = animations[i].first;
        if (type == "turtle" || type == "obstacle") {
            updateLooped(deltaTime, i);
        } else if (type == "death" || type == "score") {
            updateTemporary(deltaTime, i, window);
        } else if (type == "arrow") {
            updateArrow(deltaTime, i, window);
        }
    }
    // Remove the turtle animations that need to be removed.
    for (int j : toBeRemoved) {
        remove(j);
    }
}

/**
 * Handles the update of a looped animation. Removes if the animation is not used anymore.
 * @param deltaTime - The amount of time that has passed since last update.
 * @param id - The id of the looped animation.
 */
void AnimationHandler::updateLooped(Time deltaTime, int id) {
    animations[id].second.update(deltaTime);
    removeIfInactive(id);
}

/**
 * Handles the update of a temporary animation. Removes the animation once it is finished.
 * @param deltaTime - The amount of time that has passed since last update.
 * @param id - The id of the animation.
 * @param window - The window to be rendered in.
 */
void AnimationHandler::updateTemporary(Time deltaTime, int id, RenderWindow *window) {
    animations[id].second.update(deltaTime);
    if (!animations[id].second.isPlaying()) {
        toBeRemoved.push_back(id);
        return;
    }
    window->draw(animations[id].second);
}

/**
 * Handles the update of an arrow. If there are still arrows to be rendered,
 * create a new animation.
 * @param deltaTime - The amount of time that has passed since last update.
 * @param id - The id of the animation.
 * @param window - The window to be rendered in.
 */
void AnimationHandler::updateArrow(Time deltaTime, int id, RenderWindow *window) {
    animations[id].second.update(deltaTime);

    if (!animations[id].second.isPlaying()) {

        toBeRemoved.push_back(id);

        // If there are still arrows left to draw, add a new animation.
        if (!arrowPositions.empty() && !arrowRotations.empty()) {
            pair<float, float> pos = arrowPositions.front();
            arrowPositions.pop();
            startTemporaryAnimation(pos.first, pos.second, arrowAnimationData, "arrow", &arrowSpriteSheet,
                                    arrowRotations.front());
            arrowRotations.pop();
        }

        return;
    }
    window->draw(animations[id].second);
}


/**
 * Removes a looped animation if the elapsed time since last access is too long.
 * @param id - The id to be checked.
 */
void AnimationHandler::removeIfInactive(int id) {
    if (timeouts[id].getElapsedTime().asSeconds() > 1) {
        toBeRemoved.push_back(id);
    }
}

/**
 * Starts a temporary animation based on the parameters.
 * @param x - The x coordinate.
 * @param y - The y coordinate.
 * @param animationData - Data of the animation.
 * @param type - The type of animation.
 * @param spriteSheet - Pointer to the spritesheet as a texture object.
 * @param rotation - Rotation to rotate the animation by.
 */
void AnimationHandler::startTemporaryAnimation(float x, float y, AnimationData *animationData, string type,
                                               Texture *spriteSheet, float rotation) {
    Animation *animation = new Animation();
    animation->setFrameSize(pair<int, int>(animationData->getWidth(), animationData->getHeight()));
    animation->setSpriteSheet(*spriteSheet);

    for (IntRect rect : animationData->getFrames()) {
        animation->addFrame(rect);
    }

    // Prepare Animated Sprite
    AnimatedSprite animatedSprite = AnimatedSprite(sf::seconds(0.2), false, false, animationData->getSpeed());
    animatedSprite.setOrigin(round(animationData->getWidth() / 2), round(animationData->getHeight() / 2));
    animatedSprite.play(*animation);
    animatedSprite.setPosition(x, y);
    animatedSprite.setRotation(rotation);

    // Scale the sprite.
    float avgSize = (animationData->getWidth() + animationData->getHeight()) / 2.f;
    float scalingFactor = Variables::scaleToPixels(animationData->getSize()) / avgSize;
    animatedSprite.scale(scalingFactor, scalingFactor);

    // Add to the data.
    int id = Variables::getNewId();
    animations[id] = pair<string, AnimatedSprite>(type, animatedSprite);
    activeIds.push_back(id);
};

/**
 * Starts a looped animation based on the parameters.
 * @param id - The id of the corresponding object.
 * @param animationData - Data of the animation.
 * @param type - The type of animation.
 * @param spriteSheet - Pointer to the spritesheet as a texture object.
 */
void AnimationHandler::startLoopedAnimation(int id, AnimationData *animationData, string type, Texture *spriteSheet) {
    Animation *animation = new Animation();
    animation->setFrameSize(pair<int, int>(animationData->getWidth(), animationData->getHeight()));
    animation->setSpriteSheet(*spriteSheet);

    for (IntRect rect : animationData->getFrames()) {
        animation->addFrame(rect);
    }

    // Prepare animated sprite
    AnimatedSprite animatedSprite = AnimatedSprite(sf::seconds(0.2), false, true, animationData->getSpeed());
    animatedSprite.setOrigin(round(animationData->getWidth() / 2), round(animationData->getHeight() / 2));
    animatedSprite.play(*animation);

    // Scale to match the shape hitbox.
    float avgSize = (animationData->getWidth() + animationData->getHeight()) / 2.f;
    float scalingFactor = Variables::scaleToPixels(Variables::TURTLE_RADIUS) * 3.f / avgSize;
    animatedSprite.scale(scalingFactor, scalingFactor);

    // Add it to the data.
    animations[id] = pair<string, AnimatedSprite>(type, animatedSprite);
    timeouts[id] = Clock();
    activeIds.push_back(id);
}


/**
 * Adds a looped turtle animation to the animation map.
 * @param id - The id of the turtle to be added.
 */
void AnimationHandler::startTurtleAnimation(int id) {
    startLoopedAnimation(id, turtleAnimationData, "turtle", &turtleSpriteSheet);
}

/**
 * Adds a looped obstacle animation to the animation map.
 * @param id - The id of the obstacle to be added.
 */
void AnimationHandler::startObstacleAnimation(int id) {
    startLoopedAnimation(id, obstacleAnimationData, "obstacle", &obstacleSpriteSheet);
}

/**
 * Adds a death animation to the animation map.
 * @param x - The x position of the animation.
 * @param y - The y position of the animation.
 */
void AnimationHandler::startDeathAnimation(float x, float y) {
    startTemporaryAnimation(x, y, deathAnimationData, "death", &deathSpriteSheet, 0.f);
}

/**
 * Adds a score animation to the animation map.
 * @param x - The x position of the animation.
 * @param y - The y position of the animation.
 */
void AnimationHandler::startScoreAnimation(float x, float y) {
    startTemporaryAnimation(x, y, scoreAnimationData, "score", &scoreSpriteSheet, 0.f);
}

/**
 * Prepares the arrow animation locations and rotations, then starts 4 arrow animations,
 * each pointing to a sea.
 */
void AnimationHandler::startArrowAnimations() {
    prepareArrowAnimations();
    for (int i = 0; i < 4; i++) {
        pair<float, float> pos = arrowPositions.front();
        arrowPositions.pop();
        startTemporaryAnimation(pos.first, pos.second, arrowAnimationData, "arrow", &arrowSpriteSheet,
                                arrowRotations.front());
        arrowRotations.pop();
    }
}

/**
 * Removes an id from the animations map.
 * @param id - The id to be removed.
 */
void AnimationHandler::remove(int id) {
    if (animations[id].first == "turtle" || animations[id].first == "obstacle") {
        timeouts.erase(id);
    }
    activeIds.remove(id);
    animations.erase(id);
}

/**
 * Unpauses the animation if it was paused.
 * @param id - The id of the animation.
 */
void AnimationHandler::play(int id) {
    animations[id].second.play();
}

/**
 * Retrieves a looped animation from the animations map.
 * Also restarts the timeout for removal.
 * @param id - The animation id to be retrieved.
 * @return the animation.
 */
AnimatedSprite &AnimationHandler::getLoopedAnimation(int id, string type) {
    if (animations.find(id) == animations.end()) {
        if (type == "turtle") {
            startTurtleAnimation(id);
        } else if (type == "obstacle") {
            startObstacleAnimation(id);
        }
    }
    timeouts[id].restart();
    return animations[id].second;
}
