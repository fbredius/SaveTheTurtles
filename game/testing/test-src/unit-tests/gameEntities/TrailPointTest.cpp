//
// Created by raoul on 6-6-18.
//

#include "gtest/gtest.h"
#include "../../../../src/gameEntities/TrailPoint.h"
#include "../../../../src/storage/Variables.h"
#include "gmock/gmock.h"
#include "../../../../src/rendering/RenderWrapper.h"

/**
 * Test class for the Player class.
 */
class TrailPointTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        point1 = TrailPoint(0, 0, 0);
        point2 = TrailPoint(20, 20, 180);
    }

    TrailPoint point1 = TrailPoint(0, 0, 0);
    TrailPoint point2 = TrailPoint(20, 20, 180);
};

/**
 * Verify that the getter for the x coordinate works.
 */
TEST_F(TrailPointTest, getX) {
    EXPECT_EQ(point1.getX(), 0);
    EXPECT_EQ(point2.getX(), 20);
}

/**
 * Verify that the getter for the y coordinate works.
 */
TEST_F(TrailPointTest, getY) {
    EXPECT_EQ(point1.getY(), 0);
    EXPECT_EQ(point2.getY(), 20);
}

/**
 * Verify that the getter for the direction works.
 */
TEST_F(TrailPointTest, getDirection) {
    EXPECT_EQ(point1.getDirection(), 0);
    EXPECT_EQ(point2.getDirection(), 180);
}

/**
 * Verify that the getter for the opacity works.
 */
TEST_F(TrailPointTest, getOpacity) {
    EXPECT_EQ(point1.getOpacity(), 1.0);
    EXPECT_EQ(point2.getOpacity(), 1.0);
}

/**
 * Verify that the setter for the x coordinate works.
 */
TEST_F(TrailPointTest, setX) {
    point1.setX(4);
    point2.setX(10);

    EXPECT_EQ(point1.getX(), 4);
    EXPECT_EQ(point2.getX(), 10);
}

/**
 * Verify that the setter for the y coordinate works.
 */
TEST_F(TrailPointTest, setY) {
    point1.setY(14);
    point2.setY(110);

    EXPECT_EQ(point1.getY(), 14);
    EXPECT_EQ(point2.getY(), 110);
}

/**
 * Verify that the setter for the direction works.
 */
TEST_F(TrailPointTest, setDirection) {
    point1.setDirection(360);
    point2.setDirection(100);

    EXPECT_EQ(point1.getDirection(), 360);
    EXPECT_EQ(point2.getDirection(), 100);
}

/**
 * Verify that the timer is decreased accordingly.
 */
TEST_F(TrailPointTest, decreaseTimer) {
    point1.decreaseFadeTimer(10.0);
    point2.decreaseFadeTimer(0.5);

    EXPECT_DOUBLE_EQ(point1.getOpacity(), 1.0 - Variables::TRAIL_FADE_SPEED);
    EXPECT_DOUBLE_EQ(point2.getOpacity(), 1.0);
}

