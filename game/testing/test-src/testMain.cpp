//
// Created by maxim on 28-5-18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

/**
 * Run all tests.
 * @param argc - unused input arguments
 * @param argv - unused input arguments
 * @return returns 0 when tests pass, returns -1 if tests fail, returns other values for specific error codes.
 */
int main(int argc, char *argv[]) {
    std::cout << "Starting test runner ..." << std::endl;
    testing::InitGoogleTest(&argc, argv);

    // IMPORTANT: Comment this line if you want to activate description based tests!
    // It also disables AssetLoader tests
    // and AnimationData tests since they do not run on CI.
    ::testing::GTEST_FLAG(filter) = "*.*-*DescriptionBased_*:*AssetLoader_*:*Animation_*";

    int result = RUN_ALL_TESTS();
    std::cout << "All tests ran!" << std::endl;
    return result;
}

