//
// Created by mariette on 1-6-18.
//

#include "gtest/gtest.h"
#include "../../../../src/gameEntities/turtle/Turtle.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleIdleState.h"
#include "../../../../src/storage/Variables.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleReturningState.h"
#include "../../../../../../src/gameEntities/turtle/turtleStates/TurtleWalkState.h"
#include "../../../../../../src/gameEntities/turtle/turtleStates/TurtleObjectiveState.h"
#include <cmath>

/**
 * Testing class for the Turtle Idle state.
 */
class TurtleIdleStateTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        Variables::ID_COUNTER = 0;
        voldetort = Turtle();
        voldeIdle = new TurtleIdleState(&voldetort);
        voldetort.setCurrentState(voldeIdle);
    }

    Turtle voldetort = Turtle();
    TurtleIdleState *voldeIdle = new TurtleIdleState(&voldetort);
};

/**
 * Tests that the turtle can turn direction.
 */
TEST_F(TurtleIdleStateTest, turnTest) {
    (*voldeIdle).turn();
    EXPECT_EQ(voldetort.getWiggleFactor(), -1);
}

/**
 * Tests if all counters are properly decremented.
 */
TEST_F(TurtleIdleStateTest, counterTest) {
    voldetort.setWiggleTimer(Variables::WIGGLE_DURATION_IDLE);
    voldetort.setWiggling(true);
    InputData inputData(InputData::mouse);
    inputData.setFrameTime(0.01);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);
    (*voldeIdle).updateCounters(inputList.front());

    // Tests if the counters are updated for one iteration.
    EXPECT_DOUBLE_EQ(voldetort.getWiggleTimer(), Variables::WIGGLE_DURATION_IDLE - inputData.getFrameTime());
    EXPECT_DOUBLE_EQ(voldetort.getMoveTimer(), 0.0);

    // Tests if the turtle turns properly.
    while (voldetort.getWiggleTimer() - inputData.getFrameTime() >= 0) {
        (*voldeIdle).updateCounters(inputList.front());
    }
    (*voldeIdle).updateCounters(inputList.front());
    EXPECT_DOUBLE_EQ(voldetort.getWiggleTimer(), Variables::WIGGLE_DURATION_IDLE);
    EXPECT_EQ(voldetort.getWiggleFactor(), -1);
}

/**
 * Tests the multiple aspects of the update function.
 */
TEST_F(TurtleIdleStateTest, updateTest) {
    voldetort.setWiggleTimer(0.01);
    voldetort.setWiggling(true);
    InputData inputData(InputData::mouse);
    inputData.setFrameTime(0.001);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);

    double tempX = voldetort.getX();
    double tempY = voldetort.getY();

    (*voldeIdle).update(inputList);

    /*
     * Tests if the turtle moves into the right direction.
     * Because of rounding errors tests if the two points are sufficiently close.
     */
    EXPECT_TRUE(abs(voldetort.getX() -
                    (tempX + (voldetort.getSpeed() * inputData.getFrameTime() * cos(voldetort.getDirection()))))
                <= 0.5);
    EXPECT_TRUE(abs(voldetort.getY() -
                    (tempY + (voldetort.getSpeed() * inputData.getFrameTime() * sin(voldetort.getDirection()))))
                <= 0.5);

    // Tests if the turtle changes to a new direction.
    double oldDir = voldetort.getDirection();
    double newDir = voldetort.getDirection();

    while (abs(newDir - oldDir) <= 0.01) {
        (*voldeIdle).update(inputList);
        newDir = voldetort.getDirection();
    }
    EXPECT_TRUE(fabs(oldDir - newDir) >= 0.01);
}

/**
 * Verify that this state returns the correct StateIdentifier when called from a BasicState instance.
 */
TEST_F(TurtleIdleStateTest, StateIdentifierTest) {
    voldetort.setCurrentState(voldeIdle);
    EXPECT_EQ(voldetort.getCurrentState()->getStateIdentifier(), "1: TurtleIdleState");
}

/**
 * Verify that nothing happens with regard to state switching 
 * when a turtle collides with another turtle while in the TurtleIdleState.
 */
TEST_F(TurtleIdleStateTest, onCollideTurtle) {
    voldetort.setCurrentState(voldeIdle);
    Turtle turtle = Turtle();
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    state->onCollide(turtle);

    EXPECT_TRUE(dynamic_cast<TurtleIdleState *>(voldetort.getCurrentState()));
}

/**
 * Verify that a turtle that collides with a player while in the TurtleIdleState goes into the TurtleWalkState.
 * It should have the TURTLE_WALK_SPEED when not colliding with the inner player hitBox.
 */
TEST_F(TurtleIdleStateTest, onCollidePlayerOuter) {
    voldetort.setCurrentState(voldeIdle);
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());

    Player player = Player(1);
    list<Player *> collidedPlayers;
    collidedPlayers.push_back(&player);

    state->onCollide(collidedPlayers, false);

    EXPECT_TRUE(dynamic_cast<TurtleWalkState *>(voldetort.getCurrentState()));
    EXPECT_DOUBLE_EQ(voldetort.getSpeed(), Variables::scaleToPixels(Variables::TURTLE_WALK_SPEED));
    EXPECT_FALSE(voldetort.isReturning());
    EXPECT_FALSE(voldetort.isIdle());
    EXPECT_FALSE(voldetort.isRoaming());
    EXPECT_TRUE(voldetort.isInteractable());
    EXPECT_FALSE(voldetort.isResetting());
}

/**
 * Verify that a turtle that collides with a player while in the TurtleIdleState goes into the TurtleWalkState.
 * It should have the TURTLE_FORCE_SPEED when colliding with the inner player hitBox.
 */
TEST_F(TurtleIdleStateTest, onCollidePlayerInner) {
    voldetort.setCurrentState(voldeIdle);
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());

    Player player = Player(1);
    list<Player *> collidedPlayers;
    collidedPlayers.push_back(&player);

    state->onCollide(collidedPlayers, true);

    EXPECT_TRUE(dynamic_cast<TurtleWalkState *>(voldetort.getCurrentState()));
    EXPECT_DOUBLE_EQ(voldetort.getSpeed(), Variables::scaleToPixels(Variables::TURTLE_WALK_SPEED));
    EXPECT_FALSE(voldetort.isReturning());
    EXPECT_FALSE(voldetort.isIdle());
    EXPECT_FALSE(voldetort.isRoaming());
    EXPECT_TRUE(voldetort.isInteractable());
    EXPECT_FALSE(voldetort.isResetting());
}

/**
 * Verify that a turtle that collides with an ObjectiveArea while in the TurtleIdleState goes into
 * the TurtleObjectiveState.
 * And verify that the score is incremented.
 */
TEST_F(TurtleIdleStateTest, onCollideObjectiveArea) {
    voldetort.setCurrentState(voldeIdle);
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
 * Verify that a turtle that collides with an obstacle while in the TurtleIdleState dies.
 */
TEST_F(TurtleIdleStateTest, onCollideObstacle) {
    voldetort.setCurrentState(voldeIdle);
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());

    state->onCollideObstacle();

    EXPECT_FALSE(voldetort.isAlive());
}