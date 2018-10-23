#include <iostream>
#include <SFML/Graphics.hpp>
#include "Main.h"
#include "storage/Variables.h"
#include "control/GameManager.h"
#include "rendering/RenderWrapper.h"
#include "storage/InputData.h"
#include "control/gameStates/GameRunningState.h"
#include "gameEntities/turtle/turtleStates/TurtleState.h"
#include "rendering/AnimationHandler.h"
#include <SFML/Window.hpp>
#include <cmath>
#include <sstream>
#include <list>

using namespace std;
using namespace sf;

int main(int argc, char *argv[]) {
    openWindow();
}

/**
 * Open the game window.
 *
 * --- WINDOW CONTROLS ---
 * SPACEBAR - Next scene/start game.
 * K - Kill all players.
 * L - Add player.
 */
void openWindow() {

    Variables::IMOVE_ACTIVE = false;

    // Randomize random values.
    srand(time(NULL));

    // --- Gathered Screen Information ---
    //maxim: width=2880.72 height=1620.41
    //raoul: width=1440.36  height=810.203
    //projector: width=1920 height=1200 aspect-ratio = 16:10

    // Enable anti-aliasing.
    ContextSettings settings;
    settings.antialiasingLevel = 4;

    // Prepare classes that are needed to update the game each frame.
    GameManager gameManager = GameManager();

    //Initialize background, player and forcefield within this constructor.
    RenderWrapper *renderWrapper = new RenderWrapper();
    sf::Clock clock;
    bool playersInitialized = false;

    // Initialize window
    RenderWindow window(VideoMode(static_cast<unsigned int>(Variables::PIXEL_WIDTH),
                                  static_cast<unsigned int>(Variables::PIXEL_HEIGHT)),
                        Variables::GAME_NAME, Style::Default, settings);

    window.setFramerateLimit(Variables::FRAMERATE_LIMIT);

    // Set window after initializing renderwrapper to avoid 'Not responding' alert.
    RenderWindow *windowPointer = &window;
    renderWrapper->setWindow(windowPointer);


    // Enter the main application loop, this loop is entered upon each SFML frame.
    while (window.isOpen()) {

        InputData *inputData = new InputData(InputData::mouse);
        InputData *inputData2 = new InputData(InputData::arrowkeys);
        InputData *inputData3 = new InputData(InputData::wasdkeys);

        // Process the input received during this frame.
        sf::Event event;
        while (window.pollEvent(event)) {

            // Close the window upon the window close event.
            if (event.type == Event::Closed) {
                window.close();
            }

            // Get mouse input as a debug input of the player position.
            if (event.type == Event::MouseMoved) {
                inputData->setValid();
                inputData->setPlayerX(event.mouseMove.x);
                inputData->setPlayerY(event.mouseMove.y);
            }

            // Get keyboard input as a debug input of the player position.
            if (event.type == Event::KeyPressed) {
                // Get the x and y movement of the arrow keys, to the right and down will result in a 1, left and up -1
                int xMoveArrow = Keyboard::isKeyPressed(Keyboard::Right) - Keyboard::isKeyPressed(Keyboard::Left);
                int yMoveArrow = Keyboard::isKeyPressed(Keyboard::Down) - Keyboard::isKeyPressed(Keyboard::Up);
                // Only set the input to be valid if the player is actually moving
                if (!(xMoveArrow == 0 and yMoveArrow == 0)) {
                    inputData2->setValid();
                    inputData2->setPlayerX(10 * xMoveArrow);
                    inputData2->setPlayerY(10 * yMoveArrow);
                }
                // Get the x and y movement of the wasd keys, to the right and down will result in a 1, left and up -1
                int xMoveWASD = Keyboard::isKeyPressed(Keyboard::D) - Keyboard::isKeyPressed(Keyboard::A);
                int yMoveWASD = Keyboard::isKeyPressed(Keyboard::S) - Keyboard::isKeyPressed(Keyboard::W);
                // Only set the input to be valid if the player is actually moving
                if (!(xMoveWASD == 0 and yMoveWASD == 0)) {
                    inputData3->setValid();
                    inputData3->setPlayerX(10 * xMoveWASD);
                    inputData3->setPlayerY(10 * yMoveWASD);
                }
            }

            // Check if the spacebar is pressed. Pressing the spacebar will start/restart the game.
            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                inputData->pressSpace();
            }

            // Check if K is pressed. This will kill all players, used for debugging.
            if (Keyboard::isKeyPressed(Keyboard::K)) {
                inputData->pressK();
            }

            // Check if L is pressed. This will add a player, used for debugging.
            if (Keyboard::isKeyPressed(Keyboard::L)) {
                inputData->pressL();
            }

            // Check if J is pressed. This will toggle the DEBUG_ACTIVE variable.
            if (Keyboard::isKeyPressed(Keyboard::J)) {
                inputData->pressJ();
            }
        }

        // Get the elapsed time during the current frame and put in InputData and RenderWrapper.
        // This is used by the game logic to stabilize actions that should happen over time.
        // When multiplied by this value, the result will be constant regardless of the frame rate.
        // This is also used when fading from the running state into the finished state.
        Time frameTime = clock.restart();
        inputData->setFrameTime(frameTime.asSeconds());
        renderWrapper->setFrameTime(frameTime.asSeconds());

        // Update all game logic.
        list<InputData *> inputList;
        inputList.push_back(inputData);
        inputList.push_back(inputData2);
        inputList.push_back(inputData3);
        gameManager.update(inputList);

        // Clear the window on each tick.
        renderWrapper->clearFrame();

        // Draw background.
        renderWrapper->renderBackground();

        // Render all game entities.
        gameManager.render(renderWrapper);

        // Update the animations after the game logic is updated.
        renderWrapper->updateAnimations(frameTime, windowPointer);

        // Draw DebugInfo
        if (Variables::DEBUG_MODE) {
            renderWrapper->renderDebugInfo(inputData);
        }

        // Destroy the InputData object once it is no longer needed to free allocated memory.
        delete inputData;
        delete inputData2;
        delete inputData3;

        // Display the contents of the window that were just rendered.
        renderWrapper->displayFrame();

        // Only initialize the debugging players when IMOVE is not active, IMOVE will add players by itself if its active.
        if (!playersInitialized && !Variables::IMOVE_ACTIVE && gameManager.getPlayerManager() != nullptr) {

            // Initialize all of the player objects
            for (int i = 0; i < Variables::NUMBER_OF_PLAYERS; i++) {
                if (gameManager.getPlayerManager() != nullptr) {
                    gameManager.getPlayerManager()->createPlayer();
                }
            }

            playersInitialized = true;
        }

    }
}