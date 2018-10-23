//
// Created by thijmen on 20-6-18.
//

#include <gtest/gtest.h>
#include "../../../src/rendering/RenderWrapper.h"
#include "../../../src/storage/Variables.h"
#include "../../../src/gameEntities/turtle/Turtle.h"
#include "../../../src/gameEntities/turtle/turtleStates/TurtleRoamingState.h"
#include "../../../src/gameEntities/turtle/turtleStates/TurtleObjectiveState.h"
#include "../../../src/gameEntities/turtle/turtleStates/TurtleIdleState.h"
#include "../../../src/gameEntities/turtle/turtleStates/TurtleWalkState.h"
#include "../../../src/utility/CollisionHandler.h"
#include "../../../src/control/gameStates/GameRunningState.h"
#include "../../../src/control/gameStates/GameFinishedState.h"

using namespace std;

/**
 * Tests if the acceptance criteria for user story 2 are met.
 */
class UserStory6 : public ::testing::Test {
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
 * Tests if the score is displayed.
 */
TEST_F(UserStory6, DescriptionBased_DisplayScore) {

    std::cout << "--- DESCRIPTION BASED TEST ---" << std::endl;
    std::cout << "TestName = DisplayScore" << std::endl;
    std::cout << "The score should be shown on screen." << std::endl;

    // ----- Setup -----
    // This could not be generalized due to SFML issues.
    RenderWindow *window = new RenderWindow(
            VideoMode(static_cast<unsigned int>(1920), static_cast<unsigned int>(1200 / 2)),
            "Description Based Test Runner", Style::Default, settings);
    RenderWrapper wrapper = RenderWrapper(window);

    // Display the frame.
    while (window->isOpen()) {

        // If the timer ends, close the window.
        timer -= clock.restart().asSeconds();
        if (timer <= 0.0) {
            window->close();
        }

        Variables::resetScore();

        // --- TEST CODE ---
        wrapper.clearFrame();
        wrapper.renderBackground();
        wrapper.renderGUI();
        wrapper.displayFrame();
    }
}

/**
 * Verify that the score increments when a turtle enters the objective area.
 */
TEST_F(UserStory6, TestScoreMethods) {
    // Check if the score is initially 0.
    Variables::resetScore();
    EXPECT_EQ(0, Variables::getScore());

    // Create and initialize the input list.
    std::list<InputData *> inputList;
    InputData inputData = InputData(InputData::mouse);
    inputData.setFrameTime(0.001);
    inputList.push_back(&inputData);

    // Create a new turtle and locate it just outside of the objective area.
    Turtle *scoreTurtle = new Turtle();
    scoreTurtle->setDirection(255 * (M_PI / 180.0));
    BasicState *scoreState = new TurtleWalkState(scoreTurtle);
    scoreTurtle->setCurrentState(scoreState);
    double justOutside = sqrt(pow(Variables::scaleToPixels(Variables::OBJECTIVE_RADIUS), 2) / 2) + 1;
    scoreTurtle->setX(justOutside);
    scoreTurtle->setY(justOutside);

    // Create and initialize the northWest objective area.
    TurtleManager *tm = new TurtleManager();
    list<Turtle *> turtles = tm->getTurtles();
    turtles.push_back(scoreTurtle);
    tm->setTurtles(turtles);
    list<ObjectiveArea *> *oal = new list<ObjectiveArea *>;
    auto *northWest = new ObjectiveArea(0.0, 0.0, Variables::scaleToPixels(Variables::OBJECTIVE_RADIUS), 225.0);
    oal->push_back(northWest);
    CollisionHandler *ch = new CollisionHandler(new PlayerManager(), tm, new ObstacleManager(), *oal);

    // While the turtle is walking, update its movement until it collides with the objective area.
    while (!dynamic_cast<TurtleObjectiveState *>(scoreTurtle->getCurrentState())) {
        scoreTurtle->update(inputList);

        ch->handleCollision();
    }

    // On collision with the objective area, the score should increment.
    EXPECT_EQ(1, Variables::getScore());
}

/**
 * Verify that the score increments when a turtle enters the objective area.
 */
TEST_F(UserStory6, TestFinishingScreen) {
    // Check if the score is initially 1 below the maximum.
    Variables::setScore(Variables::TOTAL_TURTLES - 1);
    EXPECT_EQ(Variables::TOTAL_TURTLES - 1, Variables::getScore());

    // Create and initialize the input list.
    std::list<InputData *> inputList;
    InputData inputData = InputData(InputData::mouse);
    inputData.setFrameTime(0.001);
    inputList.push_back(&inputData);

    // Create a new turtle and locate it just outside of the objective area.
    Turtle *scoreTurtle = new Turtle();
    scoreTurtle->setDirection(255 * (M_PI / 180.0));
    BasicState *scoreState = new TurtleWalkState(scoreTurtle);
    scoreTurtle->setCurrentState(scoreState);
    double justOutside = sqrt(pow(Variables::scaleToPixels(Variables::OBJECTIVE_RADIUS), 2) / 2) + 1;
    scoreTurtle->setX(justOutside);
    scoreTurtle->setY(justOutside);

    // Create and initialize the northWest objective area.
    TurtleManager *tm = new TurtleManager();
    list<Turtle *> turtles = tm->getTurtles();
    turtles.push_back(scoreTurtle);
    tm->setTurtles(turtles);

    GameManager *gm = new GameManager();
    BasicState *currentState = new GameRunningState(gm, tm, gm->getPlayerManager());
    gm->setCurrentState(currentState);

    EXPECT_TRUE(dynamic_cast<GameRunningState *>(gm->getCurrentState()));

    CollisionHandler *ch = new CollisionHandler(new PlayerManager(), tm, new ObstacleManager(),
                                                gm->getObjectiveAreas());

    // While the turtle is walking, update its movement until it collides with the objective area.
    while (!dynamic_cast<TurtleObjectiveState *>(scoreTurtle->getCurrentState())) {
        scoreTurtle->update(inputList);

        ch->handleCollision();

        gm->getCurrentState()->update(inputList);
    }

    // On collision with the objective area, the score should increment.
    EXPECT_EQ(Variables::TOTAL_TURTLES, Variables::getScore());
    EXPECT_TRUE(dynamic_cast<GameFinishedState *>(gm->getCurrentState()));
}