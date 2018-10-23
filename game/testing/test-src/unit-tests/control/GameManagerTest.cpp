//
// Created by timbuntu on 17-5-18.
//

#include <SFML/Config.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../../../../src/control/GameManager.h"
#include "../../../../src/control/gameStates/GameRunningState.h"
#include "../../../../src/control/gameStates/GameIdleState.h"
#include "../../../../src/rendering/RenderWrapper.h"
#include "../../../../src/control/gameStates/GameResetState.h"
#include "../../../../src/control/gameStates/GameFinishedState.h"
#include "../../../../src/control/gameStates/GameStartState.h"

using namespace sf;
using namespace testing;

/**
 * Test class for the GameManager.
 */
class GameManagerTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        gameManager = new GameManager();
    }

    GameManager *gameManager;

    virtual void TearDown() {
        delete gameManager;
    }

};

/**
 * Tests the state getter of GameManager.
 */
TEST_F(GameManagerTest, GetGameStateTest) {
    gameManager->setCurrentState(new GameIdleState(gameManager));
    ASSERT_TRUE(dynamic_cast<GameIdleState*>(gameManager->getCurrentState()));
}

/**
 * Tests the state setters of the GameManager.
 */
TEST_F(GameManagerTest, SetGameStateTest) {
    GameManager gameManager = GameManager();
    gameManager.setCurrentState(new GameIdleState(&gameManager));
    EXPECT_TRUE(dynamic_cast<GameIdleState *>(gameManager.getCurrentState()));
}

/**
 * Tests whether the update method correctly passes the update to its state.
 */
TEST_F(GameManagerTest, UpdateTest) {
    gameManager->setCurrentState(new GameIdleState(gameManager));

    InputData *inputData = new InputData(InputData::mouse);
    inputData->pressSpace();
    list<InputData *> inputList;
    inputList.push_back(inputData);
    gameManager->update(inputList);

    if (Variables::SHOW_TUTORIAL) {
        EXPECT_TRUE(dynamic_cast<GameStartState *>(gameManager->getCurrentState()));
    } else {
        EXPECT_TRUE(dynamic_cast<GameRunningState *>(gameManager->getCurrentState()));
    }
}

/**
 * Tests retrieval of the PlayerManager from the GameManager that uses a dynamic cast of
 * a BasicState to a GameRunningState.
 */
TEST_F(GameManagerTest, GetPlayerManagerRunningStateTest) {
    gameManager->setCurrentState(new GameRunningState(gameManager));
    PlayerManager* playerManager = gameManager->getPlayerManager();
    EXPECT_EQ(playerManager->getPlayers().size(), (unsigned int) 0);
    playerManager->createPlayer();
    EXPECT_EQ(playerManager->getPlayers().size(), (unsigned int) 1);
}

/**
 * Tests retrieval of the PlayerManager from the GameManager that uses a dynamic cast of
 * a BasicState to a GameResetState.
 */
TEST_F(GameManagerTest, GetPlayerManagerResetStateTest) {
    TurtleManager turtleManager = TurtleManager();
    PlayerManager playerManager = PlayerManager();
    ObstacleManager obstacleManager = ObstacleManager();
    playerManager.createPlayer();

    gameManager->setCurrentState(new GameResetState(gameManager, &turtleManager, &playerManager, &obstacleManager));
    PlayerManager* playerManager2 = gameManager->getPlayerManager();
    EXPECT_EQ(playerManager2->getPlayers().size(), (unsigned int) 1);
    playerManager2->createPlayer();
    EXPECT_EQ(playerManager2->getPlayers().size(), (unsigned int) 2);
}

/**
 * Tests retrieval of the PlayerManager from the GameManager that uses a dynamic cast of
 * a BasicState to a GameIdleState.
 */
TEST_F(GameManagerTest, GetPlayerManagerIdleStateTest) {
    gameManager->setCurrentState(new GameIdleState(gameManager));
    PlayerManager* playerManager = gameManager->getPlayerManager();
    EXPECT_EQ(playerManager->getPlayers().size(), (unsigned int) 0);
    playerManager->createPlayer();
    EXPECT_EQ(playerManager->getPlayers().size(), (unsigned int) 1);
}

/**
 * Tests retrieval of the PlayerManager from the GameManager in the case that the current
 * state does not have a PlayerManager.
 */
TEST_F(GameManagerTest, GetPlayerManagerNullptr) {
    gameManager->setCurrentState(new GameFinishedState(gameManager));
    PlayerManager* playerManager = gameManager->getPlayerManager();
    EXPECT_EQ(playerManager, nullptr);
}