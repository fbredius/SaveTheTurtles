//
// Created by timbuntu on 22-6-18.
//

#ifndef GAME_ANIMATIONDATA_H
#define GAME_ANIMATIONDATA_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

using namespace sf;
using namespace std;

/**
 * The animation data class is a utility class used for holding data of an animation.
 */
class AnimationData {
private:
    list<IntRect> frames;
    int speed;
    int width;
    int height;
    double size;

public:
    AnimationData(int speed, int width, int height, double size);

    AnimationData();

    void addFrame(int x, int y);

    const list<IntRect> &getFrames() const;

    int getSpeed() const;

    int getWidth() const;

    int getHeight() const;

    double getSize() const;

};

#endif //GAME_ANIMATIONDATA_H
