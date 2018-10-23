//
// Created by thijmen on 1-6-18.
//

#include <gtest/gtest.h>
#include "../../../../src/storage/Variables.h"
#include "../../../../src/utility/MathUtilities.h"

/**
 * Test class for the MathUtilities class.
 */
class MathUtilitiesTest : public ::testing::Test {
};

/**
 * Test a point outside of the spawn area.
 */
TEST_F(MathUtilitiesTest, OutsideSpawnAreaTest) {
    int xLoc = 0;
    int yLoc = 0;

    EXPECT_FALSE(MathUtilities::isInSpawn(xLoc, yLoc));
}

/**
 * Test a point with only the y outside of the spawn area.
 */
TEST_F(MathUtilitiesTest, OnlyXInSpawnAreaTest) {
    int xLoc = Variables::PIXEL_WIDTH / 2;
    int yLoc = 0;

    EXPECT_FALSE(MathUtilities::isInSpawn(xLoc, yLoc));
}

/**
 * Test a point only the x outside of the spawn area.
 */
TEST_F(MathUtilitiesTest, OnlyYInSpawnAreaTest) {
    int xLoc = 0;
    int yLoc = Variables::PIXEL_HEIGHT / 2;

    EXPECT_FALSE(MathUtilities::isInSpawn(xLoc, yLoc));
}

/**
 * Test a point inside of the spawn area.
 */
TEST_F(MathUtilitiesTest, InsideSpawnAreaTest) {
    int xLoc = Variables::PIXEL_WIDTH / 2;
    int yLoc = Variables::PIXEL_HEIGHT / 2;

    EXPECT_TRUE(MathUtilities::isInSpawn(xLoc, yLoc));
}

/**
 * Tests the compute distance method.
 */
TEST_F(MathUtilitiesTest, DistanceTest) {
    ASSERT_EQ(MathUtilities::calculateDistance(make_pair(2.0, 3.0), make_pair(1.0, 1.0)), sqrt(5.0));
}

/**
 * Tests the checkspacing method when the points are too close.
 */
TEST_F(MathUtilitiesTest, CheckSpacingTest) {
    list<pair<double, double>> *testlist = new list<pair<double, double>>;
    testlist->push_back(make_pair(1.0, 1.0));
    testlist->push_back(make_pair(2.0, 2.0));
    ASSERT_FALSE(MathUtilities::checkSpacing(testlist, make_pair(3.0, 3.0)));
}

/**
 * Tests the checkspacing method when the points are too close.
 */
TEST_F(MathUtilitiesTest, CheckSpacingTrueTest) {
    list<pair<double, double>> *testlist = new list<pair<double, double>>;
    testlist->push_back(make_pair(Variables::OBSTACLE_SPACING * Variables::PIXEL_HEIGHT, 0.0));
    ASSERT_TRUE(MathUtilities::checkSpacing(testlist, make_pair(0.0, 0.0)));
}

/**
 * Tests the generatePoints method.
 */
TEST_F(MathUtilitiesTest, GeneratePointsTest) {
    int before = Variables::TOTAL_OBSTACLES;
    Variables::TOTAL_OBSTACLES = 2;

    srand(static_cast<unsigned int>(3));

    map<int, pair<double, double>> *testmap = new map<int, pair<double, double>>;
    (*testmap)[0] = make_pair(0.0, 0.0);
    (*testmap)[1] = make_pair(Variables::OBSTACLE_SPACING * Variables::PIXEL_HEIGHT, 0.0);
    (*testmap)[2] = make_pair(100.0, 100.0);
    list<pair<double, double>> *points = MathUtilities::generatePoints(testmap, 3);

    ASSERT_EQ(points->front().first, 0.0);
    ASSERT_EQ(points->front().second, 0.0);
    ASSERT_EQ(points->back().first, Variables::OBSTACLE_SPACING * Variables::PIXEL_HEIGHT);
    ASSERT_EQ(points->back().second, 0.0);

    Variables::TOTAL_OBSTACLES = before;
}

/**
 * Tests whether the right objective area is selected.
 */
TEST_F(MathUtilitiesTest, calculateClosestObjectiveAreaTest) {

    // Create a list of objective areas.
    std::list<ObjectiveArea *> objectiveAreas;
    double scaledRadius = Variables::scaleToPixels(Variables::OBJECTIVE_RADIUS);
    auto * northWest =  new ObjectiveArea(0.0, 0.0, scaledRadius, 225.0);
    auto * northEast = new ObjectiveArea(Variables::PIXEL_WIDTH, 0.0, scaledRadius, 315.0);
    objectiveAreas.push_back(northWest);
    objectiveAreas.push_back(northEast);

    MathUtilities::setObjectiveAreas(objectiveAreas);

    ObjectiveArea selected = MathUtilities::calculateClosestObjectiveArea(5.0, 5.0);

    EXPECT_DOUBLE_EQ(selected.getX(), 0.0);
    EXPECT_DOUBLE_EQ(selected.getY(), 0.0);
}

TEST_F(MathUtilitiesTest, TestLinearInterpolation1) {
    double currentTime = 0.5;
    double totalTime = 1.0;
    double startValue = -5.0;
    double endValue = -1.0;

    EXPECT_DOUBLE_EQ(MathUtilities::linearInterpolation(currentTime, totalTime, startValue, endValue), -3.0);
}

TEST_F(MathUtilitiesTest, TestLinearInterpolation2) {
    double currentTime = 0.5;
    double totalTime = 1.0;
    double startValue = -1.0;
    double endValue = -5.0;

    EXPECT_DOUBLE_EQ(MathUtilities::linearInterpolation(currentTime, totalTime, startValue, endValue), -3.0);
}

TEST_F(MathUtilitiesTest, TestLinearInterpolation3) {
    double currentTime = 0.5;
    double totalTime = 1.0;
    double startValue = -1.0;
    double endValue = 5.0;

    EXPECT_DOUBLE_EQ(MathUtilities::linearInterpolation(currentTime, totalTime, startValue, endValue), 2.0);
}

TEST_F(MathUtilitiesTest, TestLinearInterpolation4) {
    double currentTime = 0.5;
    double totalTime = 1.0;
    double startValue = 1.0;
    double endValue = -5.0;

    EXPECT_DOUBLE_EQ(MathUtilities::linearInterpolation(currentTime, totalTime, startValue, endValue), -2.0);
}