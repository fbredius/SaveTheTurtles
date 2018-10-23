//
// Created by maxim on 17-5-18.
//

#include "gtest/gtest.h"
#include "../../../../src/storage/Variables.h"

/**
 * Verify that the scoring methods in Variables work correctly.
 */
TEST(VariablesTest, TestScoreMethods) {
    Variables::setScore(0);
    EXPECT_EQ(0, Variables::getScore());
    Variables::addScore(5);
    EXPECT_EQ(5, Variables::getScore());
    Variables::addScore(-2);
    EXPECT_EQ(3, Variables::getScore());
    Variables::resetScore();
    EXPECT_EQ(0, Variables::getScore());
    Variables::setScore(100);
    EXPECT_EQ(100, Variables::getScore());
}