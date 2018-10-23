//
// Created by timbuntu on 31-5-18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../src/utility/BasicState.h"
#include "../../../../src/control/GameManager.h"
#include "../../../../src/control/gameStates/GameIdleState.h"
#include "../../../../src/control/gameStates/GameRunningState.h"
#include "../../../../src/storage/Variables.h"
#include "../../../../src/control/gameStates/GameResetState.h"

using namespace sf;
using namespace testing;

/**
 * Test class for the Game Running State.
 */
class GameRunningStateTest : public ::testing::Test {
protected:

    void SetUp() override {
        gameManager = new GameManager();
    }

    GameManager *gameManager;

    void TearDown() override {
        delete gameManager;
    }

};

/**
 * Tests the constructor of GameRunningState.
 */
TEST_F(GameRunningStateTest, ConstructorTest) {
    Variables::NUMBER_OF_PLAYERS = 2;
    Variables::IMOVE_ACTIVE = false;
    auto *runningState = new GameRunningState(gameManager);
    gameManager->setCurrentState(runningState);

    // Test turtle count.
    int totalTurtles =
            runningState->getTurtleManager()->getLiveTurtles() + runningState->getTurtleManager()->getLeftToSpawn();
    ASSERT_EQ(Variables::TOTAL_TURTLES, totalTurtles);
    ASSERT_FALSE(runningState->isFadeFinished());
}

/**
 * Tests an alternative constructor of GameRunningState.
 */
TEST_F(GameRunningStateTest, AlternativeConstructorTest) {

    auto *state = new GameRunningState(gameManager, new PlayerManager());
    gameManager->setCurrentState(state);

    // Test turtle count.
    int totalTurtles =
            state->getTurtleManager()->getLiveTurtles() + state->getTurtleManager()->getLeftToSpawn();
    ASSERT_EQ(Variables::TOTAL_TURTLES, totalTurtles);
    ASSERT_FALSE(state->isFadeFinished());
}

/**
 * Tests an alternative constructor of GameRunningState.
 */
TEST_F(GameRunningStateTest, AlternativeConstructorTest2) {

    auto *state = new GameRunningState(gameManager, new TurtleManager(), new PlayerManager());
    gameManager->setCurrentState(state);

    // Test turtle count.
    int totalTurtles =
            state->getTurtleManager()->getLiveTurtles() + state->getTurtleManager()->getLeftToSpawn();
    ASSERT_EQ(Variables::TOTAL_TURTLES, totalTurtles);
    ASSERT_FALSE(state->isFadeFinished());
}

/**
 * Test nonActivity while there are players, the state should not change.
 */
TEST_F(GameRunningStateTest, DetectNonActivtyWithoutPlayers) {
    gameManager->setCurrentState(new GameRunningState(gameManager));
    gameManager->getPlayerManager()->createPlayer();

    std::list<InputData *> inputList;
    InputData inputData = InputData(InputData::mouse);
    inputList.push_front(&inputData);
    gameManager->update(inputList);

    EXPECT_TRUE(dynamic_cast<GameRunningState *>(gameManager->getCurrentState()));
}

/**
 * Test nonActivity while there are no players with a very small amount of time passed.
 * The state should not change.
 */
TEST_F(GameRunningStateTest, DetectNonActivtyWithoutTransition) {
    gameManager->setCurrentState(new GameRunningState(gameManager));
    gameManager->getPlayerManager()->destroyAllPlayers();

    std::list<InputData *> inputList;
    InputData inputData = InputData(InputData::mouse);
    // Set the frame time so large that the NO_ACTIVITY_TIMEOUT will always be smaller.
    inputData.setFrameTime(static_cast<float>(Variables::NO_ACTIVITY_TIMEOUT - 0.1));
    inputList.push_front(&inputData);
    gameManager->update(inputList);

    EXPECT_TRUE(dynamic_cast<GameRunningState *>(gameManager->getCurrentState()));
}

/**
 * Test nonActivity while there are no players with a very large amount of time passed.
 * The state should change to the GameResetState.
 */
TEST_F(GameRunningStateTest, DetectNonActivtyWithTransition) {
    gameManager->setCurrentState(new GameRunningState(gameManager));
    gameManager->getPlayerManager()->destroyAllPlayers();

    std::list<InputData *> inputList;
    InputData inputData = InputData(InputData::mouse);
    // Set the frame time so large that the NO_ACTIVITY_TIMEOUT will never be larger.
    inputData.setFrameTime(static_cast<float>(Variables::NO_ACTIVITY_TIMEOUT + 100));
    inputList.push_front(&inputData);
    gameManager->update(inputList);

    EXPECT_TRUE(dynamic_cast<GameResetState *>(gameManager->getCurrentState()));
}

/**
 * Verify that this state returns the correct StateIdentifier when called from a BasicState instance.
 */
TEST_F(GameRunningStateTest, StateIdentifierTest) {
    gameManager->setCurrentState(new GameRunningState(gameManager));
    EXPECT_EQ(gameManager->getCurrentState()->getStateIdentifier(), "GameRunningState");
}