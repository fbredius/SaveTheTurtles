//
// Created by mariette on 5-6-18.
//

#include "gtest/gtest.h"
#include "../../../../src/gameEntities/turtle/Turtle.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleObjectiveState.h"
#include "../../../../src/storage/Variables.h"
#include <cmath>

/**
 * Testing class for the Turtle Objective state.
 */
class TurtleObjectiveStateTest : public ::testing::Test {
protected:

    void SetUp() override {
        Variables::ID_COUNTER = 0;
        objectiveArea = ObjectiveArea(1.0, 2.0, 3.0, 4.0);
        voldetort = Turtle();
        voldesea = new TurtleObjectiveState(&voldetort, objectiveArea);
        voldetort.setCurrentState(voldesea);
    }

    ObjectiveArea objectiveArea = ObjectiveArea(1.0, 2.0, 3.0, 4.0);
    Turtle voldetort = Turtle();
    TurtleObjectiveState *voldesea;
};

/**
 * Tests if the calculation for new alpha works, in other words the turtles opacity.
 * Case where the turtle is far away from the ObjectiveArea.
 */
TEST_F(TurtleObjectiveStateTest, calculateAlphaNoSeaSet) {
    EXPECT_DOUBLE_EQ(voldesea->calculateAlpha(), 255.0);
}

/**
 * Tests if the calculation for new alpha works, in other words the turtles opacity.
 * Case where the turtle is right on top of the swimDestination.
 */
TEST_F(TurtleObjectiveStateTest, calculateAlphaNorthWestTest) {
    voldetort.setX(100.75640869140625);
    voldetort.setY(8.97564697265625);
    EXPECT_DOUBLE_EQ(0.0, voldesea->calculateAlpha());
    EXPECT_DOUBLE_EQ(0.0, voldetort.getAlpha());
}

/**
 * Tests whether the turtle moves in the correct direction based on the ObjectiveArea.
 */
TEST_F(TurtleObjectiveStateTest, destinationMovementTest) {

    // Disable the border offset to prevent unwanted interference with turtle movement in this small simulation.
    double temp = Variables::TURTLE_BORDER_OFFSET;
    Variables::TURTLE_BORDER_OFFSET = 0.0;

    std::list<InputData *> inputList;
    InputData inputData = InputData(InputData::mouse);
    inputData.setFrameTime(0.001);
    inputList.push_back(&inputData);

    voldetort.setX(5.0);
    voldetort.setY(5.0);

    // Update the turtle a few times.
    voldetort.update(inputList);
    voldetort.update(inputList);
    voldetort.update(inputList);
    voldetort.update(inputList);
    voldetort.update(inputList);

    // It should move in the correct direction.
    EXPECT_NE(voldetort.getX(), 5.0);
    EXPECT_NE(voldetort.getY(), 5.0);

    // Restore to original value.
    Variables::TURTLE_BORDER_OFFSET = temp;
}

/**
 * Verify that this state returns the correct StateIdentifier when called from a BasicState instance.
 */
TEST_F(TurtleObjectiveStateTest, StateIdentifierTest) {
    EXPECT_EQ(voldetort.getCurrentState()->getStateIdentifier(), "1: TurtleObjectiveState");
}

/**
 * Verify that nothing happens with regard to state switching
 * when a turtle collides with another turtle while in the TurtleObjectiveState.
 */
TEST_F(TurtleObjectiveStateTest, onCollideTurtle) {
    Turtle turtle = Turtle();
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    state->onCollide(turtle);

    EXPECT_TRUE(dynamic_cast<TurtleObjectiveState *>(voldetort.getCurrentState()));
}

/**
 * Verify that nothing happens with regard to state switching
 * when a turtle collides with a player while in the TurtleObjectiveState.
 */
TEST_F(TurtleObjectiveStateTest, onCollidePlayer) {
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());

    Player player = Player(1);
    list<Player *> collidedPlayers;
    collidedPlayers.push_back(&player);

    state->onCollide(collidedPlayers, false);

    EXPECT_TRUE(dynamic_cast<TurtleObjectiveState *>(voldetort.getCurrentState()));
}

/**
 * Verify that nothing happens with regard to state switching
 * when a turtle collides with an objectiveArea while in the TurtleObjectiveState.
 */
TEST_F(TurtleObjectiveStateTest, onCollideObjectiveArea) {
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    state->onCollide(objectiveArea);

    EXPECT_TRUE(voldetort.isAlive());
    EXPECT_TRUE(dynamic_cast<TurtleObjectiveState *>(voldetort.getCurrentState()));
}

/**
 * Verify that nothing happens with regard to state switching
 * when a turtle collides with an obstacle while in the TurtleObjectiveState.
 */
TEST_F(TurtleObjectiveStateTest, onCollideObstacle) {
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    state->onCollideObstacle();

    EXPECT_TRUE(voldetort.isAlive());
    EXPECT_TRUE(dynamic_cast<TurtleObjectiveState *>(voldetort.getCurrentState()));
}