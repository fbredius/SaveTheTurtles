//
// Created by timbuntu on 30-5-18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../src/control/TurtleManager.h"
#include "../../../../src/storage/Variables.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleIdleState.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleReturningState.h"

/**
 * Test class for the TurtleManager.
 */
class TurtleManagerTest : public ::testing::Test {
protected:

    void SetUp() override {
        turtleManager = TurtleManager();
        Variables::ID_COUNTER = 0;
        turtleManager.createTurtle();
    }

    TurtleManager turtleManager;
};

/**
 * Tests the get- and setLiveTurtles methods.
 */
TEST_F(TurtleManagerTest, SetLiveTurtlesTest) {
    ASSERT_EQ(1, turtleManager.getLiveTurtles());
    turtleManager.setLiveTurtles(0);
    ASSERT_EQ(0, turtleManager.getLiveTurtles());
}

/**
 * Tests the get- and setLeftToSpawn methods.
 */
TEST_F(TurtleManagerTest, SetLeftToSpawnTest) {
    // One turtle has already been created in the setup.
    ASSERT_EQ(Variables::TOTAL_TURTLES - 1, turtleManager.getLeftToSpawn());
    turtleManager.setLeftToSpawn(1);
    ASSERT_EQ(1, turtleManager.getLeftToSpawn());
}

/**
 * Tests the create method.
 */
TEST_F(TurtleManagerTest, CreateTurtleTest) {
    turtleManager.createTurtle();
    ASSERT_EQ(2, turtleManager.getLiveTurtles());
    ASSERT_EQ(Variables::TOTAL_TURTLES - 2, turtleManager.getLeftToSpawn());
    ASSERT_EQ(2, turtleManager.getTurtles().back()->getId());
}

/**
 * Tests the destroy turtle method.
 */
TEST_F(TurtleManagerTest, DestroyTurtleTest) {
    turtleManager.createTurtle();
    ASSERT_EQ(2, turtleManager.getLiveTurtles());
    turtleManager.destroyTurtle(turtleManager.getTurtles().front()->getId());
    ASSERT_EQ(1, turtleManager.getLiveTurtles());
    ASSERT_EQ(2, turtleManager.getTurtles().front()->getId());
}

/**
 * Tests the checkspawn method. 
 * Checks to see if the checkspawn method correctly finds whether there is space or not.
 */
TEST_F(TurtleManagerTest, CheckSpawnTest) {
    const int coordinate = 500;
    auto *testTurtle = new Turtle();
    turtleManager.getTurtles().front()->setX(coordinate);
    turtleManager.getTurtles().front()->setY(coordinate);
    testTurtle->setX(coordinate);
    testTurtle->setY(coordinate);
    turtleManager.checkSpawnClear(testTurtle);
    ASSERT_TRUE(turtleManager.checkSpawnClear(testTurtle));
    delete testTurtle;
}

/**
 * Test automatic deletion of turtles that are out of the screen.
 * This is done by the update function.
 */
TEST_F(TurtleManagerTest, OutOfScreenDeletion) {
    EXPECT_EQ(turtleManager.getTurtles().size(), (unsigned int) 1);
    turtleManager.getTurtles().front()->setOutOfScreen();
    EXPECT_TRUE(turtleManager.getTurtles().front()->isOutOfScreen());

    std::list<InputData *> inputList;
    InputData inputData = InputData(InputData::mouse);
    inputList.push_back(&inputData);
    turtleManager.update(inputList);

    EXPECT_EQ(turtleManager.getTurtles().size(), (unsigned int) 1);
    EXPECT_EQ(turtleManager.getTurtles().front()->getId(), 2);
}

/**
 * Verify that turtles are correctly put into the reset mode when resetTurtles is called.
 */
TEST_F(TurtleManagerTest, ResetTurtles) {
    Turtle *turtle = turtleManager.getTurtles().front();
    turtle->setCurrentState(new TurtleIdleState(turtle));

    EXPECT_EQ(turtle->getSpeed(), Variables::scaleToPixels(Variables::TURTLE_IDLE_SPEED));
    EXPECT_FALSE(turtle->isResetting());

    turtleManager.resetTurtles();

    EXPECT_TRUE(dynamic_cast<TurtleReturningState *>(turtle->getCurrentState()));
    EXPECT_EQ(turtle->getSpeed(), Variables::scaleToPixels(Variables::TURTLE_RESET_SPEED));
    EXPECT_TRUE(turtle->isResetting());
}

TEST_F(TurtleManagerTest, CheckTurtlesReturnedWithoutReset) {
    EXPECT_FALSE(turtleManager.allTurtlesReturned());
}

TEST_F(TurtleManagerTest, AllTurtlesDidNotReturn) {
    turtleManager.resetTurtles();
    turtleManager.createTurtle();
    Turtle *turtle = turtleManager.getTurtles().front();
    turtle->setX(0.0);
    turtle->setY(0.0);

    EXPECT_FALSE(turtleManager.allTurtlesReturned());
}

TEST_F(TurtleManagerTest, AllTurtlesDidReturn) {
    turtleManager.resetTurtles();
    Turtle *turtle = turtleManager.getTurtles().front();
    turtle->setX(Variables::PIXEL_WIDTH / 2);
    turtle->setY(Variables::PIXEL_HEIGHT / 2);
    EXPECT_TRUE(turtleManager.allTurtlesReturned());
}
