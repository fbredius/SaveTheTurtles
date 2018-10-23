//
// Created by mariette on 1-6-18.
//

#include "gtest/gtest.h"
#include "../../../../src/gameEntities/turtle/Turtle.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleRoamingState.h"
#include "../../../../src/storage/Variables.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleReturningState.h"
#include "../../../../../../src/gameEntities/turtle/turtleStates/TurtleWalkState.h"
#include "../../../../../../src/gameEntities/turtle/turtleStates/TurtleObjectiveState.h"
#include <cmath>

/**
 * Testing class for the Turtle Roaming state.
 */
class TurtleRoamingStateTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        Variables::ID_COUNTER = 0;
        voldetort = Turtle();
        voldeRoam = new TurtleRoamingState(&voldetort);
        voldetort.setCurrentState(voldeRoam);
    }

    Turtle voldetort = Turtle();
    TurtleRoamingState *voldeRoam = new TurtleRoamingState(&voldetort);
};

/**
 * Tests if the turtle can turn direction.
 */
TEST_F(TurtleRoamingStateTest, turnTest) {
    (*voldeRoam).turn();
    EXPECT_EQ(voldetort.getWiggleFactor(), -1);
}

/**
 * Tests if all the turtle counters are properly decremented after updateCounters has been called.
 */
TEST_F(TurtleRoamingStateTest, counterTest) {
    voldetort.setWiggleTimer(Variables::WIGGLE_DURATION_WALK);
    voldetort.setWiggling(true);
    InputData inputData(InputData::mouse);
    inputData.setFrameTime(0.01);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);
    (*voldeRoam).updateCounters(inputList.front());

    // Tests if the counters are updated for one iteration.
    EXPECT_NE(voldetort.getWiggleTimer(), Variables::WIGGLE_DURATION_WALK);
    EXPECT_NE(voldetort.getMoveTimer(), Variables::MOVE_DURATION_WALK);

    // Tests if the turtle turns properly.
    while (voldetort.getWiggleTimer() - inputData.getFrameTime() >= 0) {
        (*voldeRoam).updateCounters(inputList.front());
    }
    (*voldeRoam).updateCounters(inputList.front());

    EXPECT_NE(voldetort.getWiggleTimer(), Variables::WIGGLE_DURATION_WALK);
    EXPECT_EQ(voldetort.getWiggleFactor(), -1);
}

/**
 * Tests if the turtle actually moves after being updated by its current state.
 */
TEST_F(TurtleRoamingStateTest, updateTest) {
    voldetort.setWiggleTimer(Variables::WIGGLE_DURATION_WALK);
    voldetort.setWiggling(true);
    InputData inputData(InputData::mouse);
    inputData.setFrameTime(0.01);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);

    double tempX = voldetort.getX();
    double tempY = voldetort.getY();

    double direction = voldetort.getDirection();

    (*voldeRoam).update(inputList);

    // A rotation should start.
    EXPECT_LE(fabs(voldetort.getX() - (tempX + (voldetort.getSpeed() * inputData.getFrameTime()
                                                * cos(direction)))), 0.0);
    EXPECT_LE(fabs(voldetort.getY() - (tempY + (voldetort.getSpeed() * inputData.getFrameTime()
                                                * sin(direction)))), 0.0);
}

/**
 * Verify that this state returns the correct StateIdentifier when called from a BasicState instance.
 */
TEST_F(TurtleRoamingStateTest, StateIdentifierTest) {
    voldetort.setCurrentState(new TurtleRoamingState(&voldetort));
    EXPECT_EQ(voldetort.getCurrentState()->getStateIdentifier(), "1: TurtleRoamingState");
}

/**
 * Verify that nothing happens with regard to state switching
 * when a turtle collides with another turtle while in the TurtleRoamingState.
 */
TEST_F(TurtleRoamingStateTest, onCollideTurtle) {
    voldetort.setCurrentState(new TurtleRoamingState(&voldetort));
    Turtle turtle = Turtle();
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    state->onCollide(turtle);

    EXPECT_TRUE(dynamic_cast<TurtleRoamingState *>(voldetort.getCurrentState()));
}

/**
 * Verify that a turtle that collides with a player while in the TurtleRoamingState goes into the TurtleWalkState.
 * It should have the TURTLE_WALK_SPEED when not colliding with the inner player hitBox.
 */
TEST_F(TurtleRoamingStateTest, onCollidePlayerOuter) {
    voldetort.setCurrentState(new TurtleRoamingState(&voldetort));
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
 * Verify that a turtle that collides with a player while in the TurtleRoamingState goes into the TurtleWalkState.
 * It should have the TURTLE_FORCE_SPEED when colliding with the inner player hitBox.
 */
TEST_F(TurtleRoamingStateTest, onCollidePlayerInner) {
    voldetort.setCurrentState(new TurtleRoamingState(&voldetort));
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());

    Player player = Player(1);
    list<Player *> collidedPlayers;
    collidedPlayers.push_back(&player);

    state->onCollide(collidedPlayers, true);

    EXPECT_TRUE(dynamic_cast<TurtleWalkState *>(voldetort.getCurrentState()));
    EXPECT_DOUBLE_EQ(voldetort.getSpeed(), Variables::scaleToPixels(Variables::TURTLE_FORCE_SPEED));
    EXPECT_FALSE(voldetort.isReturning());
    EXPECT_FALSE(voldetort.isIdle());
    EXPECT_FALSE(voldetort.isRoaming());
    EXPECT_TRUE(voldetort.isInteractable());
    EXPECT_FALSE(voldetort.isResetting());
}

/**
 * Verify that a turtle that collides with an ObjectiveArea while in the TurtleRoamingState goes into
 * the TurtleObjectiveState.
 * And verify that the score is incremented.
 */
TEST_F(TurtleRoamingStateTest, onCollideObjectiveArea) {
    voldetort.setCurrentState(new TurtleRoamingState(&voldetort));
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
 * Verify that a turtle that collides with an obstacle while in the TurtleRoamingState dies.
 */
TEST_F(TurtleRoamingStateTest, onCollideObstacle) {
    voldetort.setCurrentState(new TurtleRoamingState(&voldetort));
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());

    state->onCollideObstacle();

    EXPECT_FALSE(voldetort.isAlive());
}