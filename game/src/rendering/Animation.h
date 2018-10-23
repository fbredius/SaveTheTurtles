//
// Created by timbuntu on 8-6-18.
//

#ifndef GAME_ANIMATION_H
#define GAME_ANIMATION_H

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

/**
 * Please note that this class comes from https://github.com/SFML/SFML/wiki/Source:-AnimatedSprite.
 */

using namespace std;

class Animation {
public:
    Animation();

    Animation(const Animation &animation);

    void addFrame(sf::IntRect rect);

    void setSpriteSheet(const sf::Texture &texture);

    const sf::Texture *getSpriteSheet() const;

    size_t getSize() const;

    const sf::IntRect &getFrame(std::size_t n) const;

    const vector<sf::IntRect> getFrames();

private:
    vector<sf::IntRect> frames;
    const sf::Texture *texture;
    pair<int, int> frameSize;
public:
    const pair<int, int> &getFrameSize() const;

    void setFrameSize(const pair<int, int> &frameSize);
};


#endif //GAME_ANIMATION_H
