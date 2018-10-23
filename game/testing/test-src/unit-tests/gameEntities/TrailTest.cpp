//
// Created by raoul on 6-6-18.
//

#include "gtest/gtest.h"
#include "../../../../src/gameEntities/Trail.h"
#include "../../../../src/storage/Variables.h"
#include "gmock/gmock.h"
#include "../../../../src/rendering/RenderWrapper.h"

/**
 * Test class for the Player class.
 */
class TrailTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        trail1 = Trail();
        trail2 = Trail();
    }

    Trail trail1 = Trail();
    Trail trail2 = Trail();
};

/**
 * Verify that the time is correctly initialized.
 */
TEST_F(TrailTest, getTime) {
    EXPECT_DOUBLE_EQ(trail1.getTime(), 1 / Variables::TRAIL_FREQUENCY);
    EXPECT_DOUBLE_EQ(trail2.getTime(), 1 / Variables::TRAIL_FREQUENCY);
}

/**
 * Verify that the list of points is correctly initialized and empty.
 */
TEST_F(TrailTest, getTrailPoints) {
    EXPECT_EQ(trail1.getTrailPoints().empty(), true);
    EXPECT_EQ(trail2.getTrailPoints().empty(), true);
}

/**
 * Verify that the timer is updated accordingly when points are updated.
 */
TEST_F(TrailTest, updatePoints) {
    trail1.updatePoints(0);
    trail2.updatePoints(1);
    EXPECT_DOUBLE_EQ(trail1.getTime(), 1 / Variables::TRAIL_FREQUENCY);
    EXPECT_DOUBLE_EQ(trail2.getTime(), 1 / Variables::TRAIL_FREQUENCY - 1);
}

/**
 * Verify that the timer is updates accoridngly when points are removed.
 */
TEST_F(TrailTest, removePoints) {
    for (int i = 0; i <= 1 / Variables::TRAIL_FREQUENCY; i++) {
        trail1.updatePoints(1 / Variables::TRAIL_FREQUENCY);
        trail2.updatePoints(Variables::TRAIL_FREQUENCY);
    }

    EXPECT_EQ(trail1.getTrailPoints().empty(), true);
    EXPECT_DOUBLE_EQ(floor(trail1.getTime()), 0);

    EXPECT_EQ(trail2.getTrailPoints().empty(), true);
    EXPECT_DOUBLE_EQ(trail2.getTime(), -Variables::TRAIL_FREQUENCY + 1 / Variables::TRAIL_FREQUENCY);
}

/**
 * Verify that points are added accordingly when the timer has fallen below zero.
 */
TEST_F(TrailTest, update) {
    for (int i = 0; i <= 1 / Variables::TRAIL_FREQUENCY; i++) {
        trail1.updatePoints(Variables::TRAIL_FREQUENCY);
        trail2.updatePoints(Variables::TRAIL_FREQUENCY);
    }

    trail1.update(10, 10, 10);
    EXPECT_EQ(trail1.getTrailPoints().empty(), false);
    EXPECT_DOUBLE_EQ(trail1.getTime(), 1 / Variables::TRAIL_FREQUENCY);

    EXPECT_EQ(trail2.getTrailPoints().empty(), true);
    EXPECT_DOUBLE_EQ(floor(trail2.getTime()), -Variables::TRAIL_FREQUENCY);

}