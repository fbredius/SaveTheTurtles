//
// Created by timbuntu on 22-6-18.
//
#include "AnimationData.h"

/**
 * Constructor for the Animation Data class.
 * @param speed - The speed at which the animation should play.
 *                The higher this number is, the more times the diff
 *                in frrametime will be added each update.
 * @param width - The width of each sprite in the spritesheet.
 * @param height - The width of each sprite in the spritesheet.
 * @param size - The size of the drawed animation. Used for scaling.
 */
AnimationData::AnimationData(int speed, int width, int height, double size) {
    this->speed = speed;
    this->width = width;
    this->height = height;
    this->size = size;
}

/**
 * Empty constructor used for initializing the static variables.
 * Is meant to do nothing.
 */
AnimationData::AnimationData() {}

/**
 * Adds a frame to the framelist according to the x and y coordinates.
 * @param x - The x coordinate of the top left corner of the sprite.
 * @param y - The y coordinate of the top left corner of the sprite.
 */
void AnimationData::addFrame(int x, int y) {
    frames.push_back(IntRect(x, y, width, height));
}

/**
 * Getter for the list of frames used to cut out sprites for the animation.
 * @return frames - A list of all the frames.
 */
const list<IntRect> &AnimationData::getFrames() const {
    return frames;
}

/**
 * Getter for the animation speed corresponding to the animation.
 * @return speed - The animation speed .
 */
int AnimationData::getSpeed() const {
    return speed;
}

/**
 * Getter for the width of each sprite in the spritesheet.
 * @return width - The width of each sprite.
 */
int AnimationData::getWidth() const {
    return width;
}

/**
 * Getter for the height of each sprite in the spritesheet.
 * @return height - The height of each sprite.
 */
int AnimationData::getHeight() const {
    return height;
}

/**
 * Getter for the size factor used for scaling the animation.
 * @return size - The size of the animation.
 */
double AnimationData::getSize() const {
    return size;
}
