#ifndef IMOVE_GAMESCENE_H
#define IMOVE_GAMESCENE_H

#include "UserManager.h"
#include "../Scene.h"
#include "../SceneConfiguration.h"
#include "../../../../../game/src/control/GameManager.h"
#include "../../../../../game/src/rendering/RenderWrapper.h"

/**
 * This is the scene that projects the turtle game.
 * It uses the GameManager class of our main game, along with the RenderWrapper, to render the scene onto the projector.
 */
class GameScene : public Scene {
private:
    // Configuration of parameters used in the scene
    SceneConfiguration sceneConfig;

    RenderWrapper *renderWrapper;

    GameManager *gameManager;

    UserManager *users;

    sf::Clock clock;

    // Processing of detected people
    void processPeople() override;

public:
    // Constructor
    explicit GameScene(SceneConfiguration config);

    // Function to draw the scene
    void draw(sf::RenderWindow &target) override;
};

#endif //IMOVE_GAMESCENE_H
