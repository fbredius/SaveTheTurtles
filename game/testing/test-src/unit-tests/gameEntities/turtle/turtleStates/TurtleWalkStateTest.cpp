//
// Created by mariette on 1-6-18.
//

#include "gtest/gtest.h"
#include "../../../../src/gameEntities/turtle/Turtle.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleWalkState.h"
#include "../../../../src/storage/Variables.h"
#include "../../../../../../src/gameEntities/turtle/turtleStates/TurtleIdleState.h"
#include "../../../../../../src/gameEntities/turtle/turtleStates/TurtleObjectiveState.h"
#include <cmath>

/**
 * Testing class for the Turtle Walking state.
 */
class TurtleWalkStateTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        Variables::ID_COUNTER = 0;
        voldetort = Turtle();
        voldewalk = new TurtleWalkState(&voldetort);
        voldetort.setCurrentState(voldewalk);
    }

    Turtle voldetort = Turtle();
    TurtleWalkState *voldewalk = new TurtleWalkState(&voldetort);
};

/**
 * Tests if voldetort can turn.
 */
TEST_F(TurtleWalkStateTest, turnTest) {
    voldetort.setWiggleFactor(-1.0);
    EXPECT_DOUBLE_EQ(voldetort.getWiggleFactor(), -1.0);
    (*voldewalk).turn();
    EXPECT_DOUBLE_EQ(voldetort.getWiggleFactor(), 1.0);
}

/**
 * Tests if the location does not update if there is no further collision. It should continue moving on its own.
 */
TEST_F(TurtleWalkStateTest, NoCollisionUpdateTest) {
    voldetort.setX(500);
    voldetort.setY(500);
    InputData inputData(InputData::mouse);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);
    (*voldewalk).update(inputList);

    int loc_diff = abs((round(voldetort.getX()) - 500)) + abs((round(voldetort.getY())) - 500);
    EXPECT_NE(loc_diff, 0);
}

/**
 * Tests if methods involved with movement adapt the wiggleTimer.
 */
TEST_F(TurtleWalkStateTest, CollisionTest) {
    voldetort.setX(1);
    voldetort.setY(1);
    voldetort.setDirection(1);
    voldetort.setWiggleTimer(Variables::WIGGLE_DURATION_WALK);
    voldetort.setMoveTimer(Variables::MOVE_DURATION_WALK);
    voldetort.setWiggling(true);

    InputData inputData(InputData::mouse);
    inputData.setFrameTime(0.01);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);

    voldetort.update(inputList);
    double prevWiggle = voldetort.getWiggleTimer();
    voldetort.update(inputList);

    EXPECT_NE(voldetort.getWiggleTimer(), prevWiggle);
}

/**
 * Tests if the update function works correctly when the turtle is in the spawn area.
 */
TEST_F(TurtleWalkStateTest, spawnUpdateTest) {
    double xLoc = Variables::PIXEL_WIDTH / 2.0;
    double yLoc = Variables::PIXEL_HEIGHT / 2.0;

    voldetort.setX(xLoc);
    voldetort.setY(yLoc);
    voldetort.setWiggleTimer(Variables::WIGGLE_DURATION_WALK);
    voldetort.setWiggling(true);
    InputData inputData(InputData::mouse);
    inputData.setFrameTime(0.01);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);
    (*voldewalk).update(inputList);

    // Turtle remains in the walking state upon collision from spawn.
    EXPECT_TRUE(dynamic_cast<TurtleWalkState *>(voldetort.getCurrentState()));
}

/**
 * Tests if update works properly on a boundary case when the state fields are not properly aligned with the turtle.
 */
TEST_F(TurtleWalkStateTest, updateTest) {
    voldetort.setIdle(true);
    voldetort.setWiggleTimer(Variables::WIGGLE_DURATION_WALK);
    voldetort.setWiggling(true);
    InputData inputData(InputData::mouse);
    inputData.setFrameTime(0.01);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);
    (*voldewalk).update(inputList);

    EXPECT_FALSE(voldetort.isReturning());
    EXPECT_TRUE(voldetort.isIdle());
    EXPECT_FALSE(voldetort.isRoaming());
    EXPECT_TRUE(voldetort.isWiggling());
}

/**
 * Tests if counters are properly decremented in the update function.
 */
TEST_F(TurtleWalkStateTest, updateCounterTest) {
    voldetort.setWiggleTimer(Variables::WIGGLE_DURATION_WALK);
    voldetort.setWiggling(true);
    InputData inputData(InputData::mouse);
    inputData.setFrameTime(0.01);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);

    // Tests if the turtle turns properly.
    while (voldetort.getWiggleTimer() - inputData.getFrameTime() >= 0) {
        (*voldewalk).updateCounters(inputList.front());
    }
    (*voldewalk).updateCounters(inputList.front());
    EXPECT_DOUBLE_EQ(voldetort.getWiggleTimer(), Variables::WIGGLE_DURATION_WALK);
    EXPECT_DOUBLE_EQ(voldetort.getWiggleFactor(), 1.0);

    // Tests if the turtle stops moving after some time.
    while (voldetort.getMoveTimer() >= 0) {
        (*voldewalk).updateCounters(inputList.front());
    }
    EXPECT_TRUE(voldetort.isWiggling());
}

/**
 * Verify that this state returns the correct StateIdentifier when called from a BasicState instance.
 */
TEST_F(TurtleWalkStateTest, StateIdentifierTest) {
    voldetort.setCurrentState(new TurtleWalkState(&voldetort));
    EXPECT_EQ(voldetort.getCurrentState()->getStateIdentifier(), "1: TurtleWalkState");
}

/**
 * Verify that nothing happens with regard to state switching
 * when a turtle collides with another turtle while in the TurtleWalkState.
 */
TEST_F(TurtleWalkStateTest, onCollideTurtle) {
    voldetort.setCurrentState(new TurtleWalkState(&voldetort));
    Turtle turtle = Turtle();
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    state->onCollide(turtle);

    EXPECT_TRUE(dynamic_cast<TurtleWalkState *>(voldetort.getCurrentState()));
}

/**
 * Verify that a turtle that collides with a player while in the TurtleWalkingState stays in the TurtleWalkState.
 * And that its speed is adapted to TURTLE_FORCE_SPEED if it collides with inner player hitBox.
 */
TEST_F(TurtleWalkStateTest, onCollidePlayerOuter) {
    voldetort.setCurrentState(new TurtleWalkState(&voldetort));
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    voldetort.setSpeed(-1.0);

    Player player = Player(1);
    list<Player *> collidedPlayers;
    collidedPlayers.push_back(&player);
    state->onCollide(collidedPlayers, false);

    EXPECT_TRUE(dynamic_cast<TurtleWalkState *>(voldetort.getCurrentState()));
    EXPECT_DOUBLE_EQ(voldetort.getSpeed(), Variables::scaleToPixels(Variables::TURTLE_WALK_SPEED));
}

/**
 * Verify that a turtle that collides with a player while in the TurtleWalkingState stays in the TurtleWalkState.
 * And that its speed is adapted to TURTLE_FORCE_SPEED if it collides with inner player hitBox.
 */
TEST_F(TurtleWalkStateTest, onCollidePlayerInner) {
    voldetort.setCurrentState(new TurtleWalkState(&voldetort));
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    voldetort.setSpeed(-1.0);

    Player player = Player(1);
    list<Player *> collidedPlayers;
    collidedPlayers.push_back(&player);
    state->onCollide(collidedPlayers, true);

    EXPECT_TRUE(dynamic_cast<TurtleWalkState *>(voldetort.getCurrentState()));
    EXPECT_DOUBLE_EQ(voldetort.getSpeed(), Variables::scaleToPixels(Variables::TURTLE_FORCE_SPEED));
}

/**
 * Verify that a turtle that collides with an ObjectiveArea while in the TurtleWalkState goes into
 * the TurtleObjectiveState.
 * And verify that the score is incremented.
 */
TEST_F(TurtleWalkStateTest, onCollideObjectiveArea) {
    voldetort.setCurrentState(new TurtleWalkState(&voldetort));
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    ObjectiveArea objectiveArea = ObjectiveArea(1, 5, 1, 5);
    int score = Variables::getScore();

    state->onCollide(objectiveArea);

    EXPECT_TRUE(dynamic_cast<TurtleObjectiveState *>(voldetort.getCurrentState()));
    EXPECT_EQ(Variables::getScore(), score + 1);
    EXPECT_FALSE(voldetort.isAlive());
    Variables::setScore(score);
}

/**
 * Verify that a turtle that collides with an obstacle while in the TurtleWalkState dies.
 */
TEST_F(TurtleWalkStateTest, onCollideObstacle) {
    voldetort.setCurrentState(new TurtleWalkState(&voldetort));
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());

    state->onCollideObstacle();

    EXPECT_FALSE(voldetort.isAlive());
}