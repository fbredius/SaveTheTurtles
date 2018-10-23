//
// Created by maxim on 7-6-18.
//

#include <gtest/gtest.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../../../src/rendering/RenderWrapper.h"
#include "../../../src/storage/Variables.h"
#include "../../../src/gameEntities/ObjectiveArea.h"

using namespace sf;
using namespace testing;

/**
 * Tests several visual rendering aspects
 */
class RenderWindowTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        settings.antialiasingLevel = 4;

        // Define how long the window is displayed.
        timer = 5.0;
    }

    ContextSettings settings;
    double timer;
    sf::Clock clock;
};

/**
 * Tests if a player displayed in a corner of the screen.
 */
TEST_F(RenderWindowTest, DescriptionBased_RenderPlayer) {

    std::cout << "--- DESCRIPTION BASED TEST ---" << std::endl;
    std::cout << "TestName = RenderPlayer" << std::endl;
    std::cout << "A player should be displayed in the top left corner" << std::endl;
    std::cout << " " << std::endl;

    // ----- Setup -----
    // This could not be generalized due to SFML issues.
    RenderWindow *window = new RenderWindow(VideoMode(static_cast<unsigned int>(800), static_cast<unsigned int>(600)),
                                            "Description Based Test Runner", Style::Default, settings);
    RenderWrapper wrapper = RenderWrapper(window);


    // Display the frame.
    while (window->isOpen()) {

        // If the timer ends, close the window.
        timer -= clock.restart().asSeconds();
        if(timer <= 0.0) {
            window->close();
        }
        // --- TEST CODE ---
        wrapper.clearFrame();
        wrapper.renderPlayer(20.0, 30.0, true, 1.0);
        wrapper.displayFrame();
    }
}

/**
 * Tests if a player is displayed in the middle of the screen.
 */
TEST_F(RenderWindowTest, DescriptionBased_RenderPlayer2) {

    std::cout << "--- DESCRIPTION BASED TEST ---" << std::endl;
    std::cout << "TestName = renderPlayer2" << std::endl;
    std::cout << "A player should be displayed in the middle" << std::endl;

    // ----- Setup -----
    // This could not be generalized due to SFML issues.
    RenderWindow *window = new RenderWindow(VideoMode(static_cast<unsigned int>(800), static_cast<unsigned int>(600)),
                                            "Description Based Test Runner", Style::Default, settings);
    RenderWrapper wrapper = RenderWrapper(window);

    // Display the frame.
    while (window->isOpen()) {

        // If the timer ends, close the window.
        timer -= clock.restart().asSeconds();
        if(timer <= 0.0) {
            window->close();
        }

        // --- TEST CODE ---
        wrapper.clearFrame();
        wrapper.renderPlayer(400, 300, true, 1.0);
        wrapper.displayFrame();
    }
}

/**
 * Tests if a turtle egg is displayed.
 */
TEST_F(RenderWindowTest, DescriptionBased_RenderTurtleSpawnState) {

    std::cout << "--- DESCRIPTION BASED TEST ---" << std::endl;
    std::cout << "TestName = RenderTurtleSpawnState" << std::endl;
    std::cout << "A turtle in spawn state should be rendered in the middle of the screen facing upwards." << std::endl;

    // ----- Setup -----
    // This could not be generalized due to SFML issues.
    RenderWindow *window = new RenderWindow(VideoMode(static_cast<unsigned int>(800), static_cast<unsigned int>(600)),
                                            "Description Based Test Runner", Style::Default, settings);
    RenderWrapper wrapper = RenderWrapper(window);

    // Display the frame.
    while (window->isOpen()) {

        // If the timer ends, close the window.
        timer -= clock.restart().asSeconds();
        if (timer <= 0.0) {
            window->close();
        }

        // --- TEST CODE ---
        wrapper.clearFrame();
        wrapper.renderEgg(400, 300, -0.5 * M_PI, 255);
        wrapper.displayFrame();
    }
}

// Keep this commented, so this can be replaced with a up to date test later.

///**
// * Tests if a turtle in the middle of the screen is displayed.
// */
//TEST_F(RenderWindowTest, DescriptionBased_RenderTurtle) {
//
//    std::cout << "--- DESCRIPTION BASED TEST ---" << std::endl;
//    std::cout << "TestName = RenderTurtle" << std::endl;
//    std::cout << "A turtle should be rendered in the middle of the screen facing southeast." << std::endl;
//
//    // ----- Setup -----
//    // This could not be generalized due to SFML issues.
//    RenderWindow *window = new RenderWindow(VideoMode(static_cast<unsigned int>(800), static_cast<unsigned int>(600)),
//                                            "Description Based Test Runner", Style::Default, settings);
//    RenderWrapper wrapper = RenderWrapper(window);
//
//    // Display the frame.
//    while (window->isOpen()) {
//
//        // If the timer ends, close the window.
//        timer -= clock.restart().asSeconds();
//        if (timer <= 0.0) {
//            window->close();
//        }
//
//        // --- TEST CODE ---
//        wrapper.clearFrame();
//        std::string testState = "testState";
//        wrapper.renderTurtle(400, 300, 0.25 * M_PI, 255, &testState, -1);
//        wrapper.displayFrame();
//    }
//}

/**
 * Tests if objective areas are rendered in the middle of the screen.
 */
TEST_F(RenderWindowTest, DescriptionBased_RenderObjectiveAreas) {

    std::cout << "--- DESCRIPTION BASED TEST ---" << std::endl;
    std::cout << "TestName = RenderObjectiveAreas" << std::endl;
    std::cout << "An objective area should be rendered with a line in the middle indicating a direction." << std::endl;

    // ----- Setup -----
    // This could not be generalized due to SFML issues.
    RenderWindow *window = new RenderWindow(VideoMode(static_cast<unsigned int>(800),
                                                      static_cast<unsigned int>(600)), "Description Based Test Runner",
                                            Style::Default, settings);
    RenderWrapper wrapper = RenderWrapper(window);

    // Display the frame.
    while (window->isOpen()) {

        // If the timer ends, close the window.
        timer -= clock.restart().asSeconds();
        if (timer <= 0.0) {
            window->close();
        }

        // --- TEST CODE ---
        wrapper.clearFrame();
        ObjectiveArea objectiveArea = ObjectiveArea(400, 300, 200, M_PI);
        objectiveArea.render(&wrapper);
        wrapper.displayFrame();
    }
}

/**
 * Tests if the background can be rendered.
 */
TEST_F(RenderWindowTest, DescriptionBased_RenderBackground) {

    std::cout << "--- DESCRIPTION BASED TEST ---" << std::endl;
    std::cout << "TestName = RenderBackground" << std::endl;
    std::cout << "The background should be rendered" << std::endl;

    // ----- Setup -----
    // This could not be generalized due to SFML issues.
    RenderWindow *window = new RenderWindow(VideoMode(static_cast<unsigned int>(1920),
                                                      static_cast<unsigned int>(1200)), "Description Based Test Runner",
                                            Style::Default, settings);
    RenderWrapper wrapper = RenderWrapper(window);

    // Display the frame.
    while (window->isOpen()) {

        // If the timer ends, close the window.
        timer -= clock.restart().asSeconds();
        if (timer <= 0.0) {
            window->close();
        }

        // --- TEST CODE ---
        wrapper.clearFrame();
        wrapper.renderBackground();
        wrapper.displayFrame();
    }
}

/**
 * Tests if the start screen can be displayed.
 */
TEST_F(RenderWindowTest, DescriptionBased_RenderStart) {

    std::cout << "--- DESCRIPTION BASED TEST ---" << std::endl;
    std::cout << "TestName = RenderStart" << std::endl;
    std::cout << "The background should be rendered" << std::endl;

    // ----- Setup -----
    // This could not be generalized due to SFML issues.
    RenderWindow *window = new RenderWindow(VideoMode(static_cast<unsigned int>(1920), static_cast<unsigned int>(1200)),
                                            "Description Based Test Runner", Style::Default, settings);
    RenderWrapper wrapper = RenderWrapper(window);

    // Display the frame.
    while (window->isOpen()) {

        // If the timer ends, close the window.
        timer -= clock.restart().asSeconds();
        if (timer <= 0.0) {
            window->close();
        }

        // --- TEST CODE ---
        wrapper.clearFrame();
        wrapper.renderStartScreen();
        wrapper.displayFrame();
    }
}

/**
 * Tests if the win screen can be rendered.
 */
TEST_F(RenderWindowTest, DescriptionBased_RenderWin) {

    std::cout << "--- DESCRIPTION BASED TEST ---" << std::endl;
    std::cout << "TestName = RenderWin" << std::endl;
    std::cout << "The win screen should be rendered." << std::endl;

    // ----- Setup -----
    // This could not be generalized due to SFML issues.
    RenderWindow *window = new RenderWindow(VideoMode(static_cast<unsigned int>(1920), static_cast<unsigned int>(1200)),
                                            "Description Based Test Runner", Style::Default, settings);
    RenderWrapper wrapper = RenderWrapper(window);

    // Display the frame.
    while (window->isOpen()) {

        // If the timer ends, close the window.
        timer -= clock.restart().asSeconds();
        if (timer <= 0.0) {
            window->close();
        }

        // --- TEST CODE ---
        wrapper.clearFrame();
        wrapper.renderWinScreen();
        wrapper.displayFrame();
    }
}

/**
 * Tests if the lose screen can be rendered.
 */
TEST_F(RenderWindowTest, DescriptionBased_RenderLose) {

    std::cout << "--- DESCRIPTION BASED TEST ---" << std::endl;
    std::cout << "TestName = RenderLose" << std::endl;
    std::cout << "The lose screen should be rendered." << std::endl;

    // ----- Setup -----
    // This could not be generalized due to SFML issues.
    RenderWindow *window = new RenderWindow(VideoMode(static_cast<unsigned int>(1920), static_cast<unsigned int>(1200)),
                                            "Description Based Test Runner", Style::Default, settings);
    RenderWrapper wrapper = RenderWrapper(window);

    // Display the frame.
    while (window->isOpen()) {

        // If the timer ends, close the window.
        timer -= clock.restart().asSeconds();
        if (timer <= 0.0) {
            window->close();
        }

        // --- TEST CODE ---
        wrapper.clearFrame();
        wrapper.renderLoseScreen();
        wrapper.displayFrame();
    }
}