//
// Created by maxim on 9-6-18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../src/control/GameManager.h"
#include "../../../../src/control/gameStates/GameResetState.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleIdleState.h"
#include "../../../../src/control/gameStates/GameRunningState.h"
#include "../../../../src/control/gameStates/GameIdleState.h"

using namespace testing;

/**
 * Test class for the Game Rest State.
 */
class GameResetStateTest : public ::testing::Test {
protected:

    void SetUp() override {
        gameManager = new GameManager();
        turtleManager = new TurtleManager();
        playerManager = new PlayerManager();
        obstacleManager = new ObstacleManager();
        inputData = new InputData(InputData::mouse);
    }

    GameManager *gameManager;
    TurtleManager *turtleManager;
    PlayerManager *playerManager;
    ObstacleManager *obstacleManager;
    InputData *inputData;

    void TearDown() override {
        delete gameManager;
        delete turtleManager;
        delete playerManager;
        delete inputData;
    }

};

/**
 * Tests the functionality in the constructor of the GameResetState.
 */
TEST_F(GameResetStateTest, Constructor) {
    turtleManager->createTurtle();
    Turtle *turtle = turtleManager->getTurtles().front();
    turtle->setCurrentState(new TurtleIdleState(turtle));
    GameResetState gameResetState = GameResetState(gameManager, turtleManager, playerManager, obstacleManager);

    EXPECT_TRUE(turtle->isResetting());
}

/**
 * Verify that when a player enters while not all turtles have returned yet that we return to the GameRunningState.
 */
TEST_F(GameResetStateTest, UpdateWithPlayers) {
    gameManager->setCurrentState(new GameResetState(gameManager, turtleManager, playerManager, obstacleManager));
    playerManager->createPlayer();
    turtleManager->createTurtle();

    // Put a turtle outside the spawn area to enfore that not all players have returned yet.
    Turtle* turtle = turtleManager->getTurtles().front();
    turtle->setX(0.0);
    turtle->setY(0.0);

    std::list<InputData *> inputList;
    InputData inputData = InputData(InputData::mouse);
    inputList.push_back(&inputData);

    EXPECT_EQ(gameManager->getPlayerManager()->numberOfPlayers(), 1);

    gameManager->update(inputList);

    EXPECT_TRUE(dynamic_cast<GameRunningState *>(gameManager->getCurrentState()));
}

/**
 * Verify that the GameResetState goes back to the GameIdleState when all turtles have returned.
 */
TEST_F(GameResetStateTest, TurtlesReturned) {
    gameManager->setCurrentState(new GameResetState(gameManager, turtleManager, playerManager, obstacleManager));
    turtleManager->createTurtle();

    std::list<InputData *> inputList;
    InputData inputData = InputData(InputData::mouse);
    inputData.setFrameTime(Variables::OBSTACLE_FADE_SPEED + 1.0);
    inputList.push_back(&inputData);

    gameManager->update(inputList);
    gameManager->update(inputList);
    gameManager->update(inputList);
    gameManager->update(inputList);
    gameManager->update(inputList);

    EXPECT_TRUE(dynamic_cast<GameIdleState *>(gameManager->getCurrentState()));
}

/**
 * Verify that this state returns the correct StateIdentifier when called from a BasicState instance.
 */
TEST_F(GameResetStateTest, StateIdentifierTest) {
    gameManager->setCurrentState(new GameResetState(gameManager, turtleManager, playerManager, obstacleManager));
    EXPECT_EQ(gameManager->getCurrentState()->getStateIdentifier(), "GameResetState");
}