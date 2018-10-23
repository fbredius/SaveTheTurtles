//
// Created by maxim on 15-6-18.
//

#include <gtest/gtest.h>
#include "../../../../src/rendering/AssetLoader.h"

/**
 * Test class for the AssetLoader class.
 * IMPORTANT:
 * These are disabled in the test suite by default because they fail on CI due to a lack of headless mode on SFML.
 * To enable them look into testMain.cpp!
 */
class AssetLoaderTest : public ::testing::Test {
};

/**
 * Define a custom assertion to test non-termination.
 */
#define GTEST_ASSERT_NO_DEATH(statement) \
    ASSERT_EXIT({{ statement } ::exit(EXIT_SUCCESS); }, ::testing::ExitedWithCode(0), "")

/**
 * Verify that the program does not exit when creating an AssetLoader.
 * The AssetLoader kills the program is any of the assets failed to load.
 * This is by design to ensure that asset related problems immediately become apparent.
 */
TEST_F(AssetLoaderTest, AssetLoader_NonDeathTest) {
    GTEST_ASSERT_NO_DEATH({ AssetLoader(); });
}

/**
 * Verify that the program is killed when the AssetLoader fails to load textures.
 */
TEST_F(AssetLoaderTest, AssetLoader_DeathTest) {
    std::string path = "INVALID_PATH";
    ASSERT_EXIT(AssetLoader(path, false), ::testing::ExitedWithCode(1), "");
}

/**
 * Verify that the loaded textures are not empty.
 * This can be verified using the getSize method since an empty unloaded texture has a
 * size of 0.
 */
TEST_F(AssetLoaderTest, AssetLoader_TestTextureGetters) {
    AssetLoader assetLoader = AssetLoader();
    EXPECT_NE(assetLoader.getTrailTexture(1)->getSize().x, (unsigned int) 0);
    EXPECT_NE(assetLoader.getTurtleTexture()->getSize().x, (unsigned int) 0);
    EXPECT_NE(assetLoader.getEggTexture()->getSize().x, (unsigned int) 0);
    EXPECT_NE(assetLoader.getObstacleTexture()->getSize().x, (unsigned int) 0);
    EXPECT_NE(assetLoader.getStartAreaTexture()->getSize().x, (unsigned int) 0);
    EXPECT_NE(assetLoader.getStartScreenTexture()->getSize().x, (unsigned int) 0);
    EXPECT_NE(assetLoader.getWinScreenTexture()->getSize().x, (unsigned int) 0);
    EXPECT_NE(assetLoader.getLoseScreenTexture()->getSize().x, (unsigned int) 0);
    EXPECT_NE(assetLoader.getBackgroundTexture()->getSize().x, (unsigned int) 0);
}

/**
 * Verify that the loaded font is not empty.
 * This can be verified by getting the texture from the font and checking that it has
 * not size of 0.
 */
TEST_F(AssetLoaderTest, AssetLoader_TestFontGetter) {
    AssetLoader assetLoader = AssetLoader();
    EXPECT_NE(assetLoader.getGameFont()->getTexture(10).getSize().x, (unsigned int) 0);
}