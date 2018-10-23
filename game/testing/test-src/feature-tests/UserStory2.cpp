//
// Created by thijmen on 20-6-18.
//

#include <gtest/gtest.h>
#include "../../../src/rendering/RenderWrapper.h"

/**
 * Tests if the acceptance criteria for user story 2 are met.
 */
class UserStory2 : public ::testing::Test {
protected:
    virtual void SetUp() {
        settings.antialiasingLevel = 4;

        // Define how long the window is displayed.
        timer = 5.0;
    }

    ContextSettings settings;
    double timer;
    sf::Clock clock;

    virtual void TearDown() {
    }
};

/**
 * Tests if the tutorial screen can be displayed.
 */
TEST_F(UserStory2, DescriptionBased_TutorialScreen) {

    std::cout << "--- DESCRIPTION BASED TEST ---" << std::endl;
    std::cout << "TestName = TutorialScreen" << std::endl;
    std::cout << "The tutorial screen should be rendered" << std::endl;

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
        wrapper.renderExplainScreen();
        wrapper.displayFrame();
    }
}