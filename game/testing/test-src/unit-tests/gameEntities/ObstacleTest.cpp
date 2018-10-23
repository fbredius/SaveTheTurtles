//
// Created by timbuntu on 7-6-18.
//

#include <gtest/gtest.h>
#include "../../../../src/gameEntities/Obstacle.h"
#include "../../../../src/storage/Variables.h"

using namespace std;

/**
 * Test class for the Obstacle class.
 */
class ObstacleTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        Variables::ID_COUNTER = 0;
        srand(static_cast<unsigned int>(3));
        obstacle = new Obstacle(1.0, 1.0);
    }

    Obstacle *obstacle;

    virtual void TearDown() {
        delete obstacle;
    }

};

/**
 * Tests the getRotation method.
 */
TEST_F(ObstacleTest, GetRotationTest) {
    ASSERT_EQ(-24, obstacle->getRotation());
}

/**
 * Tests the getRadius method.
 */
TEST_F(ObstacleTest, GetRadiusTest) {
    ASSERT_EQ(obstacle->getRadius(), Variables::scaleToPixels(Variables::OBSTACLE_RADIUS));
}

/**
 * Tests the getId method.
 */
TEST_F(ObstacleTest, GetIdTest) {
    ASSERT_EQ(obstacle->getId(), 1);
}

/**
 * Tests the getX and getY methods.
 */
TEST_F(ObstacleTest, GetPositionTest) {
    ASSERT_EQ(1.0, obstacle->getX());
    ASSERT_EQ(1.0, obstacle->getY());
}

/**
 * Tests the getHitBox method.
 */
TEST_F(ObstacleTest, GetHitBoxTest) {
    HitBox hitBox = obstacle->getHitBox();
    HitBox hitbox1 = HitBox(Variables::scaleToPixels(Variables::OBSTACLE_RADIUS), 1.0, 1.0);
    ASSERT_EQ(hitBox.getRadius(), hitbox1.getRadius());
    ASSERT_EQ(hitBox.getX(), hitbox1.getX());
    ASSERT_EQ(hitBox.getY(), hitbox1.getY());
}