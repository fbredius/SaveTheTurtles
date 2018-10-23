//
// Created by thijmen on 30-5-18.
//

#include <gtest/gtest.h>
#include "../../../../src/storage/HitBox.h"

/**
 * Test class for the HitBox class.
 */
class HitBoxTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        hitBox = HitBox(radius, xLocation, yLocation);
    }

    double xLocation = 0.0, yLocation = 0.0, radius = 1.0;
    HitBox hitBox = HitBox(radius, xLocation, yLocation);
};

/**
 * Verify that the inputs that can be held within the object and can be set correctly.
 */
TEST_F(HitBoxTest, TestInputSetters) {
    EXPECT_EQ(hitBox.getX(), xLocation);
    EXPECT_EQ(hitBox.getY(), yLocation);
    EXPECT_EQ(hitBox.getRadius(), radius);
    hitBox.setX(50.0);
    hitBox.setY(60.0);
    hitBox.setRadius(70.0);
    EXPECT_EQ(hitBox.getX(), 50.0);
    EXPECT_EQ(hitBox.getY(), 60.0);
    EXPECT_EQ(hitBox.getRadius(), 70.0);
}