//
// Created by maxim on 12-6-18.
//

#include <gtest/gtest.h>
#include <math.h>
#include "../../../../src/gameEntities/ObjectiveArea.h"

using namespace std;

/**
 * Test class for the ObjectiveArea class.
 */
class ObjectiveAreaTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        objectiveArea = new ObjectiveArea(1.0, 2.0, 3.0, 4.0);
    }

    ObjectiveArea *objectiveArea;

    virtual void TearDown() {
        delete objectiveArea;
    }

};

/**
 * Test whether the constructor initializes all values correctly.
 */
TEST_F(ObjectiveAreaTest, TestConstructor) {
    EXPECT_DOUBLE_EQ(objectiveArea->getX(), 1.0);
    EXPECT_DOUBLE_EQ(objectiveArea->getY(), 2.0);
    EXPECT_DOUBLE_EQ(objectiveArea->getRadius(), 3.0);
    EXPECT_DOUBLE_EQ(objectiveArea->getSwimDirection(), (4.0 * (M_PI / 180.0)));
}

/**
 * Test whether the HitBox that is made contains the correct values.
 */
TEST_F(ObjectiveAreaTest, TestGetHitBox) {
    HitBox hitBox = objectiveArea->getHitBox();
    EXPECT_DOUBLE_EQ(hitBox.getX(), 1.0);
    EXPECT_DOUBLE_EQ(hitBox.getY(), 2.0);
    EXPECT_DOUBLE_EQ(hitBox.getRadius(), 3.0);
}

/**
 * Test whether the getSwimDestination calculation still functions the same.
 */
TEST_F(ObjectiveAreaTest, TestCalculateSwimDestination) {
    Vector2f destination = objectiveArea->getSwimDestination();
    EXPECT_DOUBLE_EQ(destination.x, 100.75640869140625);
    EXPECT_DOUBLE_EQ(destination.y, 8.97564697265625);
}