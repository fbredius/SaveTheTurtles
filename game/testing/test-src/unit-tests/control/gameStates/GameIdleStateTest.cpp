//
// Created by timbuntu on 31-5-18.
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
 * Test class for the Game Idle State.
 */
class GameIdleStateTest : public ::testing::Test {
protected:

    void SetUp() override {
        gameManager = new GameManager();
        inputData = new InputData(InputData::mouse);
    }

    GameManager *gameManager;
    InputData *inputData;

    void TearDown() override {
        delete gameManager;
        delete inputData;
    }

};

/**
 * Tests the update method when space has not been pressed.
 */
TEST_F(GameIdleStateTest, UpdateTest) {
    gameManager->setCurrentState(new GameIdleState(gameManager));
    ASSERT_TRUE(dynamic_cast<GameIdleState *>(gameManager->getCurrentState()));

    std::list<InputData *> inputList;
    inputList.push_front(inputData);

    gameManager->getCurrentState()->update(inputList);
    ASSERT_TRUE(dynamic_cast<GameIdleState *>(gameManager->getCurrentState()));
}

/**
 * Tests the update method when space has been pressed.
 */
TEST_F(GameIdleStateTest, UpdateStartGameTest) {
    gameManager->setCurrentState(new GameIdleState(gameManager));
    ASSERT_TRUE(dynamic_cast<GameIdleState *>(gameManager->getCurrentState()));

    std::list<InputData *> inputList;
    inputData->pressSpace();
    inputList.push_front(inputData);

    gameManager->getCurrentState()->update(inputList);
    if (Variables::SHOW_TUTORIAL) {
        EXPECT_TRUE(dynamic_cast<GameStartState *>(gameManager->getCurrentState()));
    } else {
        EXPECT_TRUE(dynamic_cast<GameRunningState *>(gameManager->getCurrentState()));
    }
}

/**
 * Verify that this state returns the correct StateIdentifier when called from a BasicState instance.
 */
TEST_F(GameIdleStateTest, StateIdentifierTest) {
    gameManager->setCurrentState(new GameIdleState(gameManager));
    EXPECT_EQ(gameManager->getCurrentState()->getStateIdentifier(), "GameIdleState");
}