//
// Created by timbuntu on 8-6-18.
//

#include <vector>
#include <iostream>
#include "Animation.h"

/**
 * Please note that this class comes from https://github.com/SFML/SFML/wiki/Source:-AnimatedSprite.
 */

/**
 * Constructor for the Animation class.
 */
Animation::Animation() {
    texture = NULL;
}

/**
 * Adds a frame to the set of frames.
 * @param rect - The rectangle that you want to cut out of the spritesheet.
 */
void Animation::addFrame(sf::IntRect rect) {
    frames.push_back(rect);
}

/**
 * Setter for the to-be-used spritesheet.
 * @param texture - The texture containing the spritesheet.
 */
void Animation::setSpriteSheet(const sf::Texture &texture) {
    this->texture = &texture;
}

/**
 * Getter for the spritesheet.
 * @return m_texture - The current spritesheet (Texture).
 */
const sf::Texture *Animation::getSpriteSheet() const {
    return texture;
}

/**
 * Getter for all of the frame rectangles.
 * @return m_frames - Vector of rectangles.
 */
const std::vector<sf::IntRect> Animation::getFrames() {
    return frames;
}

/**
 * Getter for the size of the frame vector.
 * @return size_t - The amount of frames in the animation.
 */
std::size_t Animation::getSize() const {
    return frames.size();
}

/**
 * Getter for a specific frame out of the frame vector.
 * @param n - The number of the frame.
 * @return frame - The specific frame.
 */
const sf::IntRect &Animation::getFrame(std::size_t n) const {
    return frames[n];
}

/**
 * Getter for the size of the frames.
 * @return frameSize - The size as a pair of ints.
 */
const pair<int, int> &Animation::getFrameSize() const {
    return frameSize;
}

/**
 * Setter for the size of the frames.
 * @param frameSize - The size as a pair of ints.
 */
void Animation::setFrameSize(const pair<int, int> &frameSize) {
    Animation::frameSize = frameSize;
}

