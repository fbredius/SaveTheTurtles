//
// Created by maxim on 14-6-18.
//

#include <iostream>
#include <ostream>
#include "AssetLoader.h"
#include "../storage/Variables.h"

using namespace std;

/**
 * Constructor for an AssetLoader. This class loads in all the Textures and Fonts that are needed for the game and
 * checks whether any errors occur during the loading process.
 *
 * The constructor changes the path based on whether we are exectuting from an IMOVE environment
 * or as a standalone game.
 */
AssetLoader::AssetLoader() : path("../../assets/"), loadingError(false) {
    // Set the path of the assets, this differs based on whether we are running in standalone or from IMOVE.
    if (Variables::IMOVE_ACTIVE) {
        path = "../game/assets/";
    }

    init();
}

/**
 * Constructor for an AssetLoader.
 * This variant takes in a specific path.
 * @param path_in - Path the path to the assets.
 * @param loadingError_in - Default value of the loadingError.
 */
AssetLoader::AssetLoader(std::string &path_in, bool loadingError_in) : path(path_in), loadingError(loadingError_in) {
    init();
}

/**
 * Init method that is called by all AssetLoader constructors to load and error check the loading process.
 */
void AssetLoader::init() {
    loadTextures();
    loadFont();

    // Load the assets and check whether all assets loaded with success.
    if (loadingError) {
        std::cout << "FATAL ERROR WHILE LOADING ASSETS!" << std::endl;
        exit(1);
    }
}

/**
 * Method that loads in all the textures.
 * For every texture that fails to load a print to the log is made.
 */
void AssetLoader::loadTextures() {
    // Load the Trail textures.
    for (int i = 1; i <= 3; i++) {
        auto *texture = new Texture();
        String fileName = std::to_string(i);
        texture->loadFromFile(path + "sprites/dust" + fileName + ".png");
        trails.push_back(texture);
    }

    // Load the StartingArea texture.
    startAreaTexture = new Texture();
    if (!startAreaTexture->loadFromFile(path + "sprites/startbutton.png")) {
        error("Failed to load startbutton texture!");
    }

    // Load the background texture.
    backgroundTexture = new Texture();
    if (!backgroundTexture->loadFromFile(path + "sprites/background.png")) {
        error("Failed to load background texture!");
    }

    // Load start screen texture.
    startScreenTexture = new Texture();
    if (!startScreenTexture->loadFromFile(path + "sprites/turtledream.jpeg")) {
        error("Failed to load startScreen texture!");
    }

    // Load win screen texture.
    winScreenTexture = new Texture();
    if (!winScreenTexture->loadFromFile(path + "sprites/turtleend.jpg")) {
        error("Failed to load winScreen texture!");
    }

    // Load lose screen texture.
    loseScreenTexture = new Texture();
    if (!loseScreenTexture->loadFromFile(path + "sprites/turtleend.jpg")) {
        error("Failed to load loseScreen texture!");
    }

    // Load explain screen texture.
    explainScreenTexture = new Texture();
    if (!explainScreenTexture->loadFromFile(path + "sprites/explainScreen.png")) {
        error("Failed to load explainScreen texture!");
    }

    // Load Turtle texture.
    turtleTexture = new Texture();
    if (!turtleTexture->loadFromFile(path + "sprites/turtle.png")) {
        error("Failed to load turtle texture!");
    }

    // Load egg texture.
    eggTexture = new Texture();
    if (!eggTexture->loadFromFile(path + "sprites/turtleEgg.png")) {
        error("Failed to load egg texture!");
    }

    // Load Obstacle texture.
    obstacleTexture = new Texture();
    if (!obstacleTexture->loadFromFile(path + "sprites/seaUrchin.png")) {
        error("Failed to load obstacle texture!");
    }

    longWallTexture = new Texture();
    if (!longWallTexture->loadFromFile(path + "sprites/long_wall.png")) {
        error("Failed to load longWall texture!");
    }

    shortWallTexture = new Texture();
    if (!shortWallTexture->loadFromFile(path + "sprites/short_wall.png")) {
        error("Failed to load shortWall texture!");
    }

    sponsorTexture = new Texture();
    if (!sponsorTexture->loadFromFile(path + "sprites/greenpeace.png")) {
        error("Failed to load obstacle texture!");
    }
}

/**
 * Method that loads in the font.
 * If the font fails to load then a print to the log is made.
 */
void AssetLoader::loadFont() {
    // Load the game font.
    gameFont = new Font();
    if (!gameFont->loadFromFile(path + "fonts/font1.ttf")) {
        error("Failed to load gameFont!");
    }
}

/**
 * Method for registering an error during loading.
 * The loadingError boolean is set to true and a message is printed.
 * @param message - The message that is to be printed.
 */
void AssetLoader::error(const std::string &message) {
    std::cout << "[ERROR] " << message << std::endl;
    loadingError = true;
}

/**
 * Getter for a Trail texture, there is are multiple variations of trail textures.
 * @param id - The id of the specific trail texture that was requested.
 * @return trails[id] - The texture at that index in the trails vector.
 */
Texture *AssetLoader::getTrailTexture(int id) {
    return trails[id];
}

/**
 * Getter for the visual of a Turtle.
 * @return turtleTexture - A Texture from the loaded file.
 */
Texture *AssetLoader::getTurtleTexture() const {
    return turtleTexture;
}

/**
 * Getter for the visual of an Egg.
 * @return eggTexture - A Texture from the loaded file.
 */
Texture *AssetLoader::getEggTexture() const {
    return eggTexture;
}

/**
 * Getter for the visual of an Obstacle.
 * @return obstacleTexture - A Texture from the loaded file.
 */
Texture *AssetLoader::getObstacleTexture() const {
    return obstacleTexture;
}

/**
 * Getter for the visual of a StartArea.
 * @return startAreaTexture - A Texture from the loaded file.
 */
Texture *AssetLoader::getStartAreaTexture() const {
    return startAreaTexture;
}

/**
 * Getter for the visual of the start screen.
 * @return startScreenTexture - A Texture from the loaded file.
 */
Texture *AssetLoader::getStartScreenTexture() const {
    return startScreenTexture;
}

/**
 * Getter for the visual of the win screen.
 * @return winScreenTexture - A Texture from the loaded file.
 */
Texture *AssetLoader::getWinScreenTexture() const {
    return winScreenTexture;
}

/**
 * Getter for the visual of the lose screen.
 * @return loseScreenTexture - A Texture from the loaded file.
 */
Texture *AssetLoader::getLoseScreenTexture() const {
    return loseScreenTexture;
}

/**
 * Getter for the visual of the background.
 * @return backgroundTexture - A Texture from the loaded file.
 */
Texture *AssetLoader::getBackgroundTexture() const {
    return backgroundTexture;
}

/**
 * Getter for the visual of the explain screen (GameStartState).
 * @return explainScreenTexture - A Texture from the loaded file.
 */
Texture *AssetLoader::getExplainScreenTexture() const {
    return explainScreenTexture;
}

/**
 * Getter for the visual of the longWall.
 * @return longWallTexture - A Texture from the loaded file.
 */
Texture *AssetLoader::getLongWallTexture() const {
    return longWallTexture;
}

/**
 * Getter for the visual of the shortWall.
 * @return shortWallTexture - A Texture from the loaded file.
 */
Texture *AssetLoader::getShortWallTexture() const {
    return shortWallTexture;
}

/**
 * Getter for the font that is used throughout the game.
 * @return gameFont - The font from the loaded file.
 */
Font *AssetLoader::getGameFont() const {
    return gameFont;
}

/**
 * Getter for the texture that is used for the sponsor at the end screen.
 * @return sponsorTexture - A Texture from the loaded file.
 */
Texture *AssetLoader::getSponsorTexture() const {
    return sponsorTexture;
}