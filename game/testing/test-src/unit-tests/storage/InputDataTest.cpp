//
// Created by maxim on 18-5-18.
//

#include "gtest/gtest.h"
#include "../../../../src/storage/InputData.h"

/**
 * Test class for the InputData class.
 */
class InputDataTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        inputData = InputData(InputData::mouse);
    }

    InputData inputData = InputData(InputData::mouse);
};

/**
 * Verify that the instantiation to invalid values of an InputData object behaves as expected.
 */
TEST_F(InputDataTest, TestInvalidInitialization) {
    EXPECT_EQ(inputData.isValid(), false);
    EXPECT_EQ(inputData.getPlayerX(), -1.0);
    EXPECT_EQ(inputData.getPlayerY(), -1.0);
    EXPECT_EQ(inputData.getFrameTime(), -1.f);
    EXPECT_EQ(inputData.isSpacePressed(), false);
    EXPECT_EQ(inputData.isKPressed(), false);
    EXPECT_EQ(inputData.isLPressed(), false);
    EXPECT_EQ(inputData.isJPressed(), false);
}

/**
 * Verify that validity can be set.
 */
TEST_F(InputDataTest, TestValid) {
    EXPECT_EQ(inputData.isValid(), false);
    EXPECT_EQ(inputData.getPlayerX(), -1.0);
    EXPECT_EQ(inputData.getPlayerY(), -1.0);
    inputData.setValid();
    EXPECT_EQ(inputData.isValid(), true);
}

/**
 * Verify that the inputs that can be held within the object can be set correctly.
 */
TEST_F(InputDataTest, TestInputSetters) {
    EXPECT_EQ(inputData.isValid(), false);
    EXPECT_EQ(inputData.getPlayerX(), -1.0);
    EXPECT_EQ(inputData.getPlayerY(), -1.0);
    inputData.setPlayerX(50.0);
    inputData.setPlayerY(60.0);
    EXPECT_EQ(inputData.isValid(), false);
    EXPECT_EQ(inputData.getPlayerX(), 50.0);
    EXPECT_EQ(inputData.getPlayerY(), 60.0);
}

/**
 * Verify that the frame time can be set.
 */
TEST_F(InputDataTest, TestFrametime) {
    float frameTime = 1.0f;
    inputData.setFrameTime(frameTime);
    EXPECT_EQ(inputData.getFrameTime(), frameTime);
}

/**
 * Verify that the input data id can be set.
 */
TEST_F(InputDataTest, TestID) {
    int id = 42;
    inputData.setId(42);
    EXPECT_EQ(inputData.getId(), id);
}

/**
 * Verify that the constant checking of pressing the space bar is properly set.
 */
TEST_F(InputDataTest, TestSpacePressed) {
    EXPECT_EQ(inputData.isSpacePressed(), false);
    inputData.pressSpace();
    EXPECT_EQ(inputData.isSpacePressed(), true);
}

/**
 * Verify that the enumeration of the input type is properly set.
 */
TEST_F(InputDataTest, TestInputType) {
    EXPECT_EQ(inputData.getType(), InputData::mouse);
    InputData inputData2 = InputData(InputData::wasdkeys);
    InputData inputData3 = InputData(InputData::arrowkeys);
    InputData inputData4 = InputData(InputData::imove);
    EXPECT_EQ(inputData2.getType(), InputData::wasdkeys);
    EXPECT_EQ(inputData3.getType(), InputData::arrowkeys);
    EXPECT_EQ(inputData4.getType(), InputData::imove);
}

/**
 * Verify that the setter for whether K is pressed works correctly.
 */
TEST_F(InputDataTest, TestKPress) {
    EXPECT_FALSE(inputData.isKPressed());
    inputData.pressK();
    EXPECT_TRUE(inputData.isKPressed());
}

/**
 * Verify that the setter for whether L is pressed works correctly.
 */
TEST_F(InputDataTest, TestLPress) {
    EXPECT_FALSE(inputData.isLPressed());
    inputData.pressL();
    EXPECT_TRUE(inputData.isLPressed());
}

/**
 * Verify that the setter for whether J is pressed works correctly.
 */
TEST_F(InputDataTest, TestJPress) {
    EXPECT_FALSE(inputData.isJPressed());
    inputData.pressJ();
    EXPECT_TRUE(inputData.isJPressed());
}