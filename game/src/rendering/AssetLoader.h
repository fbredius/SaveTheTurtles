//
// Created by maxim on 14-6-18.
//

#ifndef GAME_ASSETLOADER_H
#define GAME_ASSETLOADER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;

/**
 * AssetLoader object, the loading of all textures, fonts etc.
 */
class AssetLoader {
private:
    std::string path;
    Texture *turtleTexture;
    Texture *eggTexture;
    Texture *obstacleTexture;
    Texture *startAreaTexture;
    Texture *startScreenTexture;
    Texture *winScreenTexture;
    Texture *loseScreenTexture;
    Texture *backgroundTexture;
    Texture *explainScreenTexture;
    Texture *longWallTexture;
    Texture *shortWallTexture;
    Texture *sponsorTexture;
    std::vector<Texture *> trails;
    Font *gameFont;
    bool loadingError;

    void init();

    void loadTextures();

    void loadFont();

    void error(const std::string &message);

public:
    AssetLoader();

    AssetLoader(std::string &path_in, bool loadingError_in);

    Texture *getTrailTexture(int id);

    Texture *getTurtleTexture() const;

    Texture *getEggTexture() const;

    Texture *getObstacleTexture() const;

    Texture *getStartAreaTexture() const;

    Texture *getStartScreenTexture() const;

    Texture *getWinScreenTexture() const;

    Texture *getLoseScreenTexture() const;

    Texture *getBackgroundTexture() const;

    Font *getGameFont() const;

    Texture *getExplainScreenTexture() const;

    Texture *getSponsorTexture() const;

    Texture *getLongWallTexture() const;

    Texture *getShortWallTexture() const;

};

#endif //GAME_ASSETLOADER_H