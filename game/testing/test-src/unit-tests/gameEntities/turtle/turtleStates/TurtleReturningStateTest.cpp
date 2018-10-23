//
// Created by mariette on 1-6-18.
//

#include "gtest/gtest.h"
#include "../../../../src/gameEntities/turtle/Turtle.h"
#include "../../../../src/storage/Variables.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleWalkState.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleReturningState.h"
#include "../../../../../../src/utility/MathUtilities.h"
#include "../../../../../../src/gameEntities/turtle/turtleStates/TurtleIdleState.h"
#include "../../../../../../src/gameEntities/turtle/turtleStates/TurtleObjectiveState.h"
#include <cmath>

/**
 * Testing class for the Turtle returning state.
 */
class TurtleReturningStateTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        Variables::ID_COUNTER = 0;
        voldetort = Turtle();
        voldewalk = new TurtleWalkState(&voldetort);
        voldereturn = new TurtleReturningState(&voldetort);
        voldetort.setCurrentState(voldewalk);
    }

    Turtle voldetort = Turtle();
    TurtleWalkState *voldewalk = new TurtleWalkState(&voldetort);
    TurtleReturningState *voldereturn = new TurtleReturningState(&voldetort);
};

/**
 * Tests if voldereturn is properly constructed.
 */
TEST_F(TurtleReturningStateTest, constructorTest) {
    EXPECT_EQ(voldetort.getSpeed(), Variables::scaleToPixels(Variables::TURTLE_WALK_SPEED));
    EXPECT_TRUE(MathUtilities::isInSpawn(
            static_cast<int>((*voldereturn).getReturnX()), static_cast<int>((*voldereturn).getReturnY())));
    EXPECT_TRUE(voldetort.isReturning());
}

/**
 * Tests if the getters for voldetorts goals work properly.
 */
TEST_F(TurtleReturningStateTest, getReturnTest) {
    EXPECT_TRUE(MathUtilities::isInSpawn(
            static_cast<int>((*voldereturn).getReturnX()), static_cast<int>((*voldereturn).getReturnY())));
}

/**
 * Tests if the setters for voldetorts goal work properly.
 */
TEST_F(TurtleReturningStateTest, setReturnTest) {
    int xLoc = Variables::PIXEL_WIDTH / 2;
    int yLoc = Variables::PIXEL_HEIGHT / 2;

    (*voldereturn).setReturnX(xLoc);
    (*voldereturn).setReturnY(yLoc);

    EXPECT_DOUBLE_EQ((*voldereturn).getReturnX(), xLoc);
    EXPECT_DOUBLE_EQ((*voldereturn).getReturnY(), yLoc);
}

/**
 * Tests if the location setters work properly when set to out of screen values.
 */
TEST_F(TurtleReturningStateTest, SetPositionObjectiveTest) {
    voldetort.setX(-5);
    EXPECT_TRUE(dynamic_cast<TurtleReturningState *>(voldetort.getCurrentState()));
}

/**
 * Tests if the location setters work properly when set to out of screen values.
 */
TEST_F(TurtleReturningStateTest, SetPositionObjectiveTest2) {
    voldetort.setY(-5);
    EXPECT_TRUE(dynamic_cast<TurtleReturningState *>(voldetort.getCurrentState()));
}

/**
 * Tests if all counters are properly decremented.
 */
TEST_F(TurtleReturningStateTest, counterTest) {
    voldetort.setX(-5);
    voldetort.setWiggleTimer(Variables::WIGGLE_DURATION_RETURNING);
    voldetort.setWiggling(true);
    InputData inputData(InputData::mouse);
    inputData.setFrameTime(0.01);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);
    (*voldereturn).updateCounters(inputList.front());

    // Tests if the counters are updated for one iteration.
    EXPECT_DOUBLE_EQ(voldetort.getWiggleTimer(), Variables::WIGGLE_DURATION_RETURNING - inputData.getFrameTime());
    EXPECT_DOUBLE_EQ(voldetort.getWiggleFactor(), -1.0);

    // Tests if the turtle turns properly.
    while (voldetort.getWiggleTimer() - inputData.getFrameTime() >= 0) {
        (*voldereturn).updateCounters(inputList.front());
    }
    (*voldereturn).updateCounters(inputList.front());
    EXPECT_DOUBLE_EQ(voldetort.getWiggleTimer(), Variables::WIGGLE_DURATION_RETURNING);
    EXPECT_DOUBLE_EQ(voldetort.getWiggleFactor(), 1.0);
}

/**
 * Tests if the move function of voldetort works properly.
 */
TEST_F(TurtleReturningStateTest, moveTest) {
    voldetort.setX(-5);
    voldetort.setWiggleTimer(Variables::WIGGLE_DURATION_RETURNING);
    voldetort.setWiggling(true);
    InputData inputData(InputData::mouse);
    inputData.setFrameTime(0.01);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);

    double tempX = voldetort.getX();
    double tempY = voldetort.getY();

    (*voldereturn).move(inputList.front());

    /*
     * Tests if the turtle moves into the right direction.
     * Because of rounding errors tests if the two points are sufficiently close.
     */
    EXPECT_TRUE(abs(voldetort.getX() -
                    (tempX + (voldetort.getSpeed() * inputData.getFrameTime() * cos(voldetort.getDirection()))))
                <= 2);
    EXPECT_TRUE(abs(voldetort.getY() -
                    (tempY + (voldetort.getSpeed() * inputData.getFrameTime() * sin(voldetort.getDirection()))))
                <= 2);
    // Tests if the turtle moves to the spawn area.
    bool inSpawn = MathUtilities::isInSpawn(
            static_cast<int>(voldetort.getX()), static_cast<int>(voldetort.getY()));

    while (!inSpawn) {
        (*voldereturn).move(inputList.front());
        inSpawn = MathUtilities::isInSpawn(
                static_cast<int>(voldetort.getX()), static_cast<int>(voldetort.getY()));
    }
    // Tests if voldetort becomes idle when moving into the spawn area.
    (*voldereturn).move(inputList.front());
    EXPECT_TRUE(dynamic_cast<TurtleIdleState *>(voldetort.getCurrentState()));
}

/**
 * Tests if the turtles are moved when update is called.
 */
TEST_F(TurtleReturningStateTest, updateTest) {
    voldetort.setX(-5);
    voldetort.setY(0);
    voldetort.setWiggleTimer(Variables::WIGGLE_DURATION_RETURNING);
    voldetort.setWiggling(true);

    InputData inputData(InputData::mouse);
    inputData.setFrameTime(static_cast<float>(Variables::WIGGLE_DURATION_RETURNING + 0.5));
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);
    (*voldereturn).update(inputList);
    (*voldereturn).update(inputList);
    (*voldereturn).update(inputList);

    double loc_diff = -5 - voldetort.getX() + voldetort.getY();
    EXPECT_NE(loc_diff, 0);;
}

/**
 * Verify that this state returns the correct StateIdentifier when called from a BasicState instance.
 */
TEST_F(TurtleReturningStateTest, StateIdentifierTest) {
    voldetort.setCurrentState(new TurtleReturningState(&voldetort));
    EXPECT_EQ(voldetort.getCurrentState()->getStateIdentifier(), "1: TurtleReturningState");
}

/**
 * Verify that nothing happens with regard to state switching
 * when a turtle collides with another turtle while in the TurtleReturningState.
 */
TEST_F(TurtleReturningStateTest, onCollideTurtle) {
    voldetort.setCurrentState(voldereturn);
    Turtle turtle = Turtle();
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    state->onCollide(turtle);

    EXPECT_TRUE(dynamic_cast<TurtleReturningState *>(voldetort.getCurrentState()));
}

/**
 * Verify that nothing happens with regard to state switching
 * when a turtle collides with a player while in the TurtleReturningState.
 */
TEST_F(TurtleReturningStateTest, onCollidePlayer) {
    voldetort.setCurrentState(voldereturn);
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());

    Player player = Player(1);
    list<Player *> collidedPlayers;
    collidedPlayers.push_back(&player);
    state->onCollide(collidedPlayers, false);

    EXPECT_TRUE(dynamic_cast<TurtleReturningState *>(voldetort.getCurrentState()));
}

/**
 * Verify that a turtle that collides with an ObjectiveArea while in the TurtleReturningState goes into
 * the TurtleObjectiveState.
 * And verify that the score is incremented.
 */
TEST_F(TurtleReturningStateTest, onCollideObjectiveArea) {
    voldetort.setCurrentState(voldereturn);

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
 * Verify that nothing happens with regard to state switching
 * when a turtle collides with an obstacle while in the TurtleReturningState.
 */
TEST_F(TurtleReturningStateTest, onCollideObstacle) {
    voldetort.setCurrentState(voldereturn);
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    state->onCollideObstacle();

    EXPECT_TRUE(voldetort.isAlive());
    EXPECT_TRUE(dynamic_cast<TurtleReturningState *>(voldetort.getCurrentState()));
}