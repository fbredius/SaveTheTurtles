//
// Created by maxim on 18-5-18.
//

#include "gtest/gtest.h"
#include "../../../../src/control/PlayerManager.h"
#include "../../../../src/utility/CollisionHandler.h"


/**
 * Testing class for the Player Manager.
 */
class PlayerManagerTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        playerManager = PlayerManager();
        inputData = InputData(InputData::mouse);
        inputData.setPlayerX(50.0);
        inputData.setPlayerY(10.0);
    }

    PlayerManager playerManager = PlayerManager();
    InputData inputData = InputData(InputData::mouse);
};

/**
 * Verify that nextId increments correctly and starts at 0.
 */
TEST_F(PlayerManagerTest, TestNextId) {
    EXPECT_EQ(playerManager.nextId(), 0);
    EXPECT_EQ(playerManager.nextId(), 1);
}

/**
 * Verify that player creation gets assigned correct IDs and is correctly added to the list of players.
 */
TEST_F(PlayerManagerTest, TestCreatePlayer) {
    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 0);
    playerManager.nextId();
    playerManager.nextId();
    playerManager.createPlayer();
    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 1);
    EXPECT_EQ(playerManager.getPlayers().front()->getId(), 2);
}

/**
 * Verify that players added to the PlayerManager get correctly updated when Update is called with VALID InputData.
 */
TEST_F(PlayerManagerTest, TestUpdatePlayersValid) {
    playerManager.createPlayer();
    playerManager.createPlayer();
    std::list<Player *> players = playerManager.getPlayers();
    inputData.setValid();

    std::list<InputData *> inputList;
    inputList.push_back(&inputData);
    playerManager.update(inputList);

    EXPECT_EQ(players.front()->getX(), 50.0);
    EXPECT_EQ(players.front()->getY(), 10.0);
    EXPECT_EQ((int) players.front()->getDirection(), -119);
    players.pop_front();

    EXPECT_EQ(players.front()->getX(), 100.0);
    EXPECT_EQ(players.front()->getY(), 100.0);
    EXPECT_EQ((int) players.front()->getDirection(), 0);
}

/**
 * Verify that players added to the PlayerManager do not updated when Update is called with INVALID InputData.
 */
TEST_F(PlayerManagerTest, TestUpdatePlayersInvalid) {
    playerManager.createPlayer();
    playerManager.createPlayer();
    std::list<Player *> players = playerManager.getPlayers();

    std::list<InputData *> inputList;
    inputList.push_back(&inputData);
    playerManager.update(inputList);

    EXPECT_EQ(players.front()->getX(), 100.0);
    EXPECT_EQ(players.front()->getY(), 100.0);
    EXPECT_EQ((int) players.front()->getDirection(), 0);
    players.pop_front();

    EXPECT_EQ(players.front()->getX(), 100.0);
    EXPECT_EQ(players.front()->getY(), 100.0);
    EXPECT_EQ((int) players.front()->getDirection(), 0);
}

/**
 * Verify that player objects can be edited/accessed outside of the PlayerManager.
 */
TEST_F(PlayerManagerTest, TestGetPlayers) {
    playerManager.createPlayer();
    playerManager.createPlayer();
    std::list<Player *> players = playerManager.getPlayers();

    players.front()->setX(-1);

    EXPECT_EQ(playerManager.getPlayers().front()->getX(), -1);
}

/**
 * Verify that players can be destroyed correctly.
 */
TEST_F(PlayerManagerTest, TestDestroyPlayer) {
    playerManager.createPlayer();
    playerManager.createPlayer();
    playerManager.createPlayer();

    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 3);

    playerManager.destroyPlayer(1);

    std::list<Player *> players = playerManager.getPlayers();
    EXPECT_EQ(players.size(), (unsigned int) 2);
    EXPECT_EQ(players.front()->getId(), 0);
    players.pop_front();
    EXPECT_EQ(players.front()->getId(), 2);

    playerManager.destroyPlayer(0);
    EXPECT_EQ(players.size(), (unsigned int) 1);
    EXPECT_EQ(playerManager.getPlayers().front()->getId(), 2);
}

/**
 * Verify that nothing happens when a non-existing ID is attempted to be destroyed.
 */
TEST_F(PlayerManagerTest, TestDestroyNonExistingPlayer) {
    playerManager.createPlayer();
    playerManager.createPlayer();
    playerManager.createPlayer();

    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 3);

    playerManager.destroyPlayer(60);

    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 3);
}

/**
 * Verify that the update method is cancelled when the list of input is empty.
 * This can be checked by verifying that debug variables are not updated.
 */
TEST_F(PlayerManagerTest, TestEmptyInputList) {
    std::list<InputData *> inputList;
    Variables::DEBUG_PLAYER_COUNT = -1;
    playerManager.update(inputList);

    EXPECT_EQ(Variables::DEBUG_PLAYER_COUNT, -1);
}

/**
 * Verify that the debug input key K destroys all players.
 */
TEST_F(PlayerManagerTest, TestDebugInputK) {
    std::list<InputData *> inputList;
    inputData.pressK();
    inputList.push_back(&inputData);

    playerManager.createPlayer();
    playerManager.createPlayer();
    playerManager.createPlayer();

    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 3);

    playerManager.update(inputList);

    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 0);
}

/**
 * Verify that the debug input key L adds a player.
 */
TEST_F(PlayerManagerTest, TestDebugInputL) {
    std::list<InputData *> inputList;
    inputData.pressL();
    inputList.push_back(&inputData);

    playerManager.createPlayer();
    playerManager.createPlayer();
    playerManager.createPlayer();

    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 3);

    playerManager.update(inputList);

    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 4);
}

/**
 * Verify that the method for creating a player with a specific Id indeed creates a player with that Id.
 */
TEST_F(PlayerManagerTest, TestCreatePlayerWithId) {
    playerManager.createPlayer(600);

    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 1);
    EXPECT_EQ(playerManager.getPlayers().front()->getId(), 600);
}

/**
 * Verify that the GetPlayer method automatically creates a player when a non-existant player is requested.
 */
TEST_F(PlayerManagerTest, TestGetNonExistantPlayer) {
    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 0);
    playerManager.getPlayer(500);

    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 1);
    EXPECT_EQ(playerManager.getPlayers().front()->getId(), 500);
}

/**
 * Verify that the getPlayer method returns the correct player.
 */
TEST_F(PlayerManagerTest, TestGetExistingPlayer) {
    playerManager.createPlayer(500);
    playerManager.getPlayer(500);

    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 1);
    EXPECT_EQ(playerManager.getPlayer(500)->getId(), 500);
}

/**
 * Verify that the destroyAllPlayers method clears the list of players.
 */
TEST_F(PlayerManagerTest, TestDestroyAllPlayers) {
    playerManager.createPlayer();
    playerManager.createPlayer();
    playerManager.createPlayer();

    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 3);
    playerManager.destroyAllPlayers();
    EXPECT_EQ(playerManager.getPlayers().size(), (unsigned int) 0);
}