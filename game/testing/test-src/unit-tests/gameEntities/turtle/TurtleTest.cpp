//
// Created by mariette on 18-5-18.
//

#include "gtest/gtest.h"
#include "../../../../src/gameEntities/turtle/Turtle.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleWalkState.h"
#include "../../../../src/storage/Variables.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleReturningState.h"
#include <cmath>

/**
 * Testing class for the Turtle class.
 */
class TurtleTest : public ::testing::Test {
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
 * Turtle copy constructor test.
 */
TEST_F(TurtleTest, CopyTurtleTest) {
    Turtle *voldecopy = new Turtle(voldetort);
    EXPECT_EQ(1, voldecopy->getId());
}

/**
 * Tests if the direction is properly calculated for boundary cases.
 */
TEST_F(TurtleTest, calculateDirectionTest) {
    double temp = voldetort.getDirection();
    voldetort.setX(100);
    voldetort.setY(100);
    EXPECT_DOUBLE_EQ(temp, voldetort.calculateDirection(100, 100));

    EXPECT_DOUBLE_EQ(1.5 * M_PI, voldetort.calculateDirection(100, 150));

    EXPECT_DOUBLE_EQ(0.5 * M_PI, voldetort.calculateDirection(100, 50));

    voldetort.setX(0);
    voldetort.setY(0);
    EXPECT_DOUBLE_EQ(atan(1), voldetort.calculateDirection(1, 1));
}

/**
 * Turtle alpha getter.
 */
TEST_F(TurtleTest, GetSetAlphaTest) {
    EXPECT_EQ(voldetort.getAlpha(), 255.0);
    voldetort.setAlpha(0.0);
    EXPECT_EQ(voldetort.getAlpha(), 0.0);
}

/**
 * Test for id getter.
 */
TEST_F(TurtleTest, GetIdTest) {
    EXPECT_EQ(1, voldetort.getId());
}

/**
 * Test for speed getter.
 */
TEST_F(TurtleTest, GetSpeedTest) {
    EXPECT_EQ(voldetort.getSpeed(), Variables::scaleToPixels(Variables::TURTLE_WALK_SPEED));
}

/**
 * Test for wiggling getter.
 */
TEST_F(TurtleTest, GetWigglingTest) {
    EXPECT_FALSE(voldetort.isWiggling());
}

/**
 * Test for wiggleTime getter.
 */
TEST_F(TurtleTest, GetWiggleTimeTest) {
    voldetort.setWiggleTimer(0.0);
    EXPECT_DOUBLE_EQ(voldetort.getWiggleTimer(), 0.0);
}

/**
 * Test for wiggleFactor getter and turning.
 */
TEST_F(TurtleTest, GetWiggleFactorTest) {
    voldetort.setWiggleFactor(1.0);
    EXPECT_DOUBLE_EQ(voldetort.getWiggleFactor(), 1.0);
}

/**
 * Test for timer getter.
 */
TEST_F(TurtleTest, GetTimerTest) {
    voldetort.setMoveTimer(0.0);
    EXPECT_DOUBLE_EQ(voldetort.getMoveTimer(), 0.0);
}

/**
 * Test for radius getter.
 */
TEST_F(TurtleTest, GetRadiusTest) {
    EXPECT_EQ(Variables::scaleToPixels(Variables::TURTLE_RADIUS), voldetort.getRadius());
}

/**
 * Test for out of screen getter.
 */
TEST_F(TurtleTest, IsOutOfScreenTest) {
    EXPECT_EQ(voldetort.isOutOfScreen(), false);

    voldetort.setOutOfScreen();
    EXPECT_EQ(voldetort.isOutOfScreen(), true);
}

/**
 * Test for returning getter.
 */
TEST_F(TurtleTest, IsReturningTest) {
    EXPECT_EQ(voldetort.isReturning(), false);

    voldetort.setReturning(true);
    EXPECT_EQ(voldetort.isReturning(), true);
}

/**
 * Test for collision getter.
 */
TEST_F(TurtleTest, IsCollidingTest) {
    EXPECT_EQ(voldetort.isColliding(), false);
}

/**
 * Tests if the constructors actually breathes life into voldetort.
 */
TEST_F(TurtleTest, IsAliveTest) {
    EXPECT_TRUE(voldetort.isAlive());
}
/**
 * Tests if the constructor sets the direction to a valid value.
 */
TEST_F(TurtleTest, RandomDirectionTest) {
    EXPECT_TRUE(0 <= voldetort.getDirection() && voldetort.getDirection() <= 2 * M_PI);
}

/**
 * Test for id setter.
 */
TEST_F(TurtleTest, SetIdTest) {
    voldetort.setId(5);
    EXPECT_EQ(5, voldetort.getId());
}

/**
 * Test for the alive boolean setter.
 */
TEST_F(TurtleTest, SetAliveTest) {
    voldetort.setAlive(false);
    EXPECT_FALSE(voldetort.isAlive());
}

/**
 * Test for timer setter.
 */
TEST_F(TurtleTest, SetTimerTest) {
    voldetort.setMoveTimer(100);
    EXPECT_EQ(voldetort.getMoveTimer(), 100);
}

/**
 * Test for wiggling setter
 * Tests if wiggleTime is also updated.
 */
TEST_F(TurtleTest, SetWigglingTest) {
    voldetort.setWiggling(true);
    EXPECT_TRUE(voldetort.isWiggling());
}

/**
 * Test for wiggleTime setter.
 */
TEST_F(TurtleTest, SetWiggleTimeTest) {
    voldetort.setWiggleTimer(50);
    EXPECT_EQ(voldetort.getWiggleTimer(), 50);
}

/**
 * Tests if the setter for the turtle's direction works.
 */
TEST_F(TurtleTest, SetDirectionTest) {
    voldetort.setDirection(1.5);
    EXPECT_EQ(1.5, voldetort.getDirection());
}

/**
 * Tests if the location setters work properly.
 */
TEST_F(TurtleTest, SetPositionTest) {
    voldetort.setX(5);
    voldetort.setY(10);
    EXPECT_DOUBLE_EQ(voldetort.getX(), 5);
    EXPECT_DOUBLE_EQ(voldetort.getY(), 10);
}

/**
 * Tests if we can kill voldetort.
 */
TEST_F(TurtleTest, KillTest) {
    voldetort.kill();
    EXPECT_FALSE(voldetort.isAlive());
}
