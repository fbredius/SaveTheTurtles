//
// Created by maxim on 18-5-18.
//

#include "gtest/gtest.h"
#include "../../../../src/gameEntities/Player.h"
#include "../../../../src/storage/Variables.h"
#include "gmock/gmock.h"
#include "../../../../src/rendering/RenderWrapper.h"


/**
 * Test class for the Player class.
 */
class PlayerTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        player = Player(1);
        player2 = Player(2);
    }

    Player player = Player(1);
    Player player2 = Player(2);
};

/**
 * Tests the constructor by instantiating two player objects.
 */
TEST_F(PlayerTest, GetIdTest) {
    EXPECT_EQ(player.getId(), 1);
    EXPECT_EQ(player2.getId(), 2);
}

/**
 * Verify that the player is active upon construction.
 */
TEST_F(PlayerTest, TestActive) {
    EXPECT_TRUE(player.isActive());
}

/**
 * Verify that the active status of players can be adapted.
 */
TEST_F(PlayerTest, TestActiveSet) {
    player.setActive(false);
    EXPECT_FALSE(player.isActive());
}

/**
 * Verify that the setters for positioning the player work as expected.
 */
TEST_F(PlayerTest, TestPositioningSetters) {
    EXPECT_EQ(player.getX(), 100.0);
    EXPECT_EQ(player.getY(), 100.0);
    EXPECT_EQ(player.getDirection(), 0.0);

    player.setX(5.0);
    player.setY(6.0);
    player.setDirection(7.0);

    EXPECT_EQ(player.getX(), 5.0);
    EXPECT_EQ(player.getY(), 6.0);
    EXPECT_EQ(player.getDirection(), 7.0);
}

/**
 * Verify that the force collision conditional is working as expected.
 * It should initialize as false.
 */
TEST_F(PlayerTest, TestForceColliding) {
    EXPECT_FALSE(player.isForceColliding());
    player.setForceColliding(true);
    EXPECT_TRUE(player.isForceColliding());
}

/**
 * Verify that the wall collision conditional is working as expected.
 * It should initialize as false.
 */
TEST_F(PlayerTest, TestWallColliding) {
    EXPECT_FALSE(player.isWallColliding());
    player.setWallColliding(true);
    EXPECT_TRUE(player.isWallColliding());
}

/**
 * Verify that the update method correctly rotates the player.
 */
TEST_F(PlayerTest, TestUpdate) {
    EXPECT_EQ(player.getX(), 100.0);
    EXPECT_EQ(player.getY(), 100.0);
    EXPECT_EQ(player.getDirection(), 0.0);

    player.update(102.1, 90.0, 0);

    EXPECT_EQ(player.getX(), 102.1);
    EXPECT_EQ(player.getY(), 90.0);
    EXPECT_GE(player.getDirection(), -78.1404);
    EXPECT_LE(player.getDirection(), -78.1400);
}

/**
 * Verify that the update method correctly rotates the player with another type of input.
 */
TEST_F(PlayerTest, TestUpdate2) {
    EXPECT_EQ(player.getX(), 100.0);
    EXPECT_EQ(player.getY(), 100.0);

    player.update(102.1, 90.0, 1);

    // Here because it's not mouse movement, we add the old location + the proposed new one to update.
    EXPECT_EQ(player.getX(), 202.1);
    EXPECT_EQ(player.getY(), 190.0);
}