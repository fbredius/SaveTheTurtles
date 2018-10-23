//
// Created by timbuntu on 8-6-18.
//

#include <iostream>
#include "AnimatedSprite.h"
#include "../storage/Variables.h"

/**
 * Please note that this class comes from https://github.com/SFML/SFML/wiki/Source:-AnimatedSprite.
 */

/**
 * Constructor for the Animated Sprite class.
 * @param frameTime - The current frame time.
 * @param paused - If the animated sprite is paused.
 * @param looped - If the animated sprite is looped.
 * @param animationSpeed - The speed of the animation. This is not the official speed,
 * just how many times the frameTime is updated with each iteration.
 */
AnimatedSprite::AnimatedSprite(sf::Time frameTime, bool isPaused, bool isLooped, int animationSpeed) :
        animation(NULL), frameTime(frameTime), currentFrame(0), paused(isPaused), looped(isLooped),
        texture(NULL) {
    if (animationSpeed < 1) {
        this->animationSpeed = 1;
    } else {
        this->animationSpeed = animationSpeed;
    }
}


/**
 * Sets the animation of the animated sprite.
 * @param animation - The animation to be set to.
 */
void AnimatedSprite::setAnimation(const Animation &animation) {
    this->animation = &animation;
    texture = this->animation->getSpriteSheet();
    currentFrame = 0;
    setFrame(currentFrame);
}

/**
 * Sets the current frame time.
 * @param time - The time to be set.
 */
void AnimatedSprite::setFrameTime(sf::Time time) {
    frameTime = time;
}

/**
 * Plays the sprite, sets paused to false.
 */
void AnimatedSprite::play() {
    paused = false;
}

/**
 * Sets the animation of the animated sprite and plays it.
 * @param animation - The animation to be set to.
 */
void AnimatedSprite::play(const Animation &animation) {
    if (getAnimation() != &animation)
        setAnimation(animation);
    play();
}

/**
 * Pauses the animation.
 */
void AnimatedSprite::pause() {
    paused = true;
}

/**
 * Stops the animation. Sets the current frame back to the beginning.
 */
void AnimatedSprite::stop() {
    paused = true;
    currentFrame = 0;
    setFrame(currentFrame);
}

/**
 * Makes the animation loop or not.
 * @param looped - To loop or not to loop.
 */
void AnimatedSprite::setLooped(bool looped) {
    this->looped = looped;
}

/**
 * Sets the color of the sprite.
 * @param color - The color to be set to.
 */
void AnimatedSprite::setColor(const sf::Color &color) {
    // Update the vertices' color
    vertices[0].color = color;
    vertices[1].color = color;
    vertices[2].color = color;
    vertices[3].color = color;
}

/**
 * Retrieves the current animation of the animated sprite.
 * @return m_animation - the current animation.
 */
const Animation *AnimatedSprite::getAnimation() const {
    return animation;
}

/**
 * Retrieves the size of the original cut out sprite.
 * @return the size.
 */
sf::FloatRect AnimatedSprite::getLocalBounds() const {
    sf::IntRect rect = animation->getFrame(currentFrame);

    float width = static_cast<float>(std::abs(rect.width));
    float height = static_cast<float>(std::abs(rect.height));

    return sf::FloatRect(0.f, 0.f, width, height);
}

/**
 * Getter for the actual hitbox of the animated sprite.
 * @return the hitbox - In the form of (x, y, width, height).
 */
sf::FloatRect AnimatedSprite::getGlobalBounds() const {
    return getTransform().transformRect(getLocalBounds());
}

/**
 * Getter to know whether the sprite is looped or not.
 * @return boolean - To denote whether or not the Animated Sprite is looped.
 */
bool AnimatedSprite::isLooped() const {
    return looped;
}

/**
 * Getter to know whether the sprite is playing or not.
 * @return boolean - To denote whether or not the Animated Sprite is playing.
 */
bool AnimatedSprite::isPlaying() const {
    return !paused;
}

/**
 * Getter for the frame time.
 * @return m_frameTime - The frametime.
 */
sf::Time AnimatedSprite::getFrameTime() const {
    return frameTime;
}

/**
 * Sets the current frame to the frame with the newFrame index.
 * @param newFrame - The frame number to be set to.
 * @param resetTime - Boolean to reset the time or not.
 */
void AnimatedSprite::setFrame(std::size_t newFrame, bool resetTime) {
    if (animation) {
        // Calculate new vertex positions and texture coordinates.
        sf::IntRect rect = animation->getFrame(newFrame);

        vertices[0].position = sf::Vector2f(0.f, 0.f);
        vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
        vertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
        vertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

        float left = static_cast<float>(rect.left) + 0.0001f;
        float right = left + static_cast<float>(rect.width);
        float top = static_cast<float>(rect.top);
        float bottom = top + static_cast<float>(rect.height);

        vertices[0].texCoords = sf::Vector2f(left, top);
        vertices[1].texCoords = sf::Vector2f(left, bottom);
        vertices[2].texCoords = sf::Vector2f(right, bottom);
        vertices[3].texCoords = sf::Vector2f(right, top);
    }

    if (resetTime) {
        currentTime = sf::Time::Zero;
    }
}

/**
 * Updates the animated sprite.
 * @param deltaTime - The amount of time passed since last update.
 */
void AnimatedSprite::update(sf::Time deltaTime) {
    // If not paused and we have a valid animation.
    if (!paused && animation) {

        // Add the delta time multiple times so the animation is faster.
        for (int i = 0; i < animationSpeed; i++) {
            currentTime += deltaTime;
        }

        // If current time is bigger then the frame time advance one frame.
        if (currentTime.asSeconds() >= frameTime.asSeconds()) {
            // Reset time, but keep the remainder.
            currentTime = sf::microseconds(currentTime.asMicroseconds() % frameTime.asMicroseconds());
            // Get next Frame index.
            if (currentFrame + 1 < animation->getSize()) {
                currentFrame++;
            } else {
                // The animation has ended.
                currentFrame = 0; // Reset to start.
                if (!looped) {
                    paused = true;
                    return;
                }
            }
            // Set the current frame, not resetting the time.
            setFrame(currentFrame, false);
            this->play(*animation);
        }
    }

}

/**
 * Draw method, which needs to be implemented in order to be a drawable.
 * Do not use this though, much easier to straight up call window.draw().
 * @param target - The target in which to draw.
 * @param states - The states used for drawing to a RenderTarget.
 */
void AnimatedSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (animation && texture) {
        states.transform *= getTransform();
        states.texture = texture;
        target.draw(vertices, 4, sf::Quads, states);
    }
}

/**
 * Getter for the currently used texture.
 * @return m_texture - The texture.
 */
const Texture *AnimatedSprite::getTexture() {
    return texture;
}

/**
 * Getter for the number of the current frame.
 * @return m_currentFrame - The current frame.
 */
std::size_t AnimatedSprite::getCurrentFrame() {
    return currentFrame;
}