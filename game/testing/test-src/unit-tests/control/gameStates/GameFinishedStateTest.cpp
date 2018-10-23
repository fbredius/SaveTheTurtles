//
// Created by timbuntu on 31-5-18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../src/utility/BasicState.h"
#include "../../../../src/control/GameManager.h"
#include "../../../../src/control/gameStates/GameIdleState.h"
#include "../../../../src/control/gameStates/GameRunningState.h"
#include "../../../../src/control/gameStates/GameFinishedState.h"
#include "../../../../src/storage/Variables.h"

using namespace sf;
using namespace testing;

/**
 * Test class for the GameManager.
 */
class GameFinishedStateTest : public ::testing::Test {
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
TEST_F(GameFinishedStateTest, UpdateTest) {

    auto *finishedState = new GameFinishedState(gameManager);
    gameManager->setCurrentState(finishedState);
    ASSERT_TRUE(dynamic_cast<GameFinishedState *>(gameManager->getCurrentState()));

    std::list<InputData *> inputList;
    inputList.push_front(inputData);
    gameManager->update(inputList);

    ASSERT_TRUE(dynamic_cast<GameFinishedState *>(gameManager->getCurrentState()));
}

/**
 * Tests the update method when space has been pressed.
 */
TEST_F(GameFinishedStateTest, UpdateWithSpaceTest) {

    auto *finishedState = new GameFinishedState(gameManager);
    gameManager->setCurrentState(finishedState);
    Variables::setScore(5);
    ASSERT_TRUE(dynamic_cast<GameFinishedState *>(gameManager->getCurrentState()));

    std::list<InputData *> inputList;
    inputData->pressSpace();
    inputList.push_front(inputData);
    gameManager->update(inputList);

    ASSERT_TRUE(dynamic_cast<GameIdleState *>(gameManager->getCurrentState()));
    ASSERT_EQ(0, Variables::getScore());
}

/**
 * Verify that this state returns the correct StateIdentifier when called from a BasicState instance.
 */
TEST_F(GameFinishedStateTest, StateIdentifierTest) {
    gameManager->setCurrentState(new GameFinishedState(gameManager));
    EXPECT_EQ(gameManager->getCurrentState()->getStateIdentifier(), "GameFinishedState");
}