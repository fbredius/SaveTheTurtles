//
// Created by maxim on 17-6-18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../src/utility/BasicState.h"
#include "../../../../src/control/GameManager.h"
#include "../../../../src/control/gameStates/GameIdleState.h"
#include "../../../../src/control/gameStates/GameRunningState.h"
#include "../../../../../src/control/gameStates/GameStartState.h"

using namespace sf;
using namespace testing;

/**
 * Test class for the Game Start State.
 */
class GameStartStateTest : public ::testing::Test {
protected:

    void SetUp() override {
        gameManager = new GameManager();
        playerManager = new PlayerManager();
        inputData = new InputData(InputData::mouse);
    }

    GameManager *gameManager;
    PlayerManager *playerManager;
    InputData *inputData;

    void TearDown() override {
        delete gameManager;
        delete playerManager;
        delete inputData;
    }

};

/**
 * Tests the update method when not enough time passed to transition to the GameRunningState.
 */
TEST_F(GameStartStateTest, UpdateTestNoTransition) {
    gameManager->setCurrentState(new GameStartState(gameManager, playerManager));
    ASSERT_TRUE(dynamic_cast<GameStartState *>(gameManager->getCurrentState()));

    std::list<InputData *> inputList;
    inputData->setFrameTime(0.0);
    inputList.push_front(inputData);

    gameManager->getCurrentState()->update(inputList);
    ASSERT_TRUE(dynamic_cast<GameStartState *>(gameManager->getCurrentState()));
}

/**
 * Tests the update method when enough time passed to transition to the GameRunningState.
 */
TEST_F(GameStartStateTest, UpdateTestTransition) {
    gameManager->setCurrentState(new GameStartState(gameManager, playerManager));
    ASSERT_TRUE(dynamic_cast<GameStartState *>(gameManager->getCurrentState()));

    std::list<InputData *> inputList;
    inputData->setFrameTime(static_cast<float>(Variables::START_SCREEN_TIME + 1.0));
    inputList.push_front(inputData);

    gameManager->getCurrentState()->update(inputList);
    ASSERT_TRUE(dynamic_cast<GameRunningState *>(gameManager->getCurrentState()));
}

/**
 * Verify that this state returns the correct StateIdentifier when called from a BasicState instance.
 */
TEST_F(GameStartStateTest, StateIdentifierTest) {
    gameManager->setCurrentState(new GameStartState(gameManager, playerManager));
    EXPECT_EQ(gameManager->getCurrentState()->getStateIdentifier(), "GameStartState");
}