//
// Created by timbuntu on 22-6-18.
//

#include <gtest/gtest.h>
#include "../../../../src/rendering/AnimationData.h"

/**
 * Test class for the AnimationData class.
 */
class AnimationDataTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        animationData = AnimationData(1, 1, 1, 4.0);
    }

    AnimationData animationData;
};

/**
 * Tests the addframe method.
 */
TEST_F(AnimationDataTest, Animation_AddFrameTest) {
    IntRect rect = IntRect(1, 1, 1, 1);
    animationData.addFrame(1, 1);
    ASSERT_EQ(animationData.getFrames().front(), rect);
}

/**
 * Tests the addframe method.
 */
TEST_F(AnimationDataTest, Animation_GetFramesTest) {
    IntRect rect = IntRect(1, 1, 1, 1);
    list<IntRect> rects;
    rects.push_back(rect);
    animationData.addFrame(1, 1);
    ASSERT_EQ(animationData.getFrames(), rects);
}

/**
 * Tests the getSpeed method.
 */
TEST_F(AnimationDataTest, Animation_GetSpeedTest) {
    ASSERT_EQ(animationData.getSpeed(), 1);
}

/**
 * Tests the getWidth method.
 */
TEST_F(AnimationDataTest, Animation_GetWidthTest) {
    ASSERT_EQ(animationData.getWidth(), 1);
}

/**
 * Tests the getHeight method.
 */
TEST_F(AnimationDataTest, Animation_GetHeightTest) {
    ASSERT_EQ(animationData.getHeight(), 1);
}

/**
 * Tests the getSize method.
 */
TEST_F(AnimationDataTest, Animation_GetSizeTest) {
    ASSERT_DOUBLE_EQ(animationData.getSize(), 4.0);

}