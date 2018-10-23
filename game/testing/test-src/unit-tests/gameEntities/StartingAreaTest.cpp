//
// Created by maxim on 9-6-18.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../../../../src/gameEntities/StartingArea.h"
#include "../../../../src/storage/Variables.h"

/**
 * Test class for the StartingArea class.
 */
class StartingAreaTest : public ::testing::Test {
protected:

    void SetUp() override {
        startingArea = new StartingArea(0, 5, 0, 5);
    }

    StartingArea* startingArea;

    void TearDown() override {
        delete startingArea;
    }

};

/**
 * By default the StartingArea should not be activated.
 */
TEST_F(StartingAreaTest, ConstructorTest) {
    EXPECT_FALSE(startingArea->isActivated());
}

/**
 * Verify that when a player is in the starting area for long enough that the StartingArea is activated.
 */
TEST_F(StartingAreaTest, ActivateSinglePlayer) {
    list<Player *> players;
    Player player1 = Player(1);
    players.push_back(&player1);

    // The starting area becomes visible after this time.
    startingArea->updateTimers(players, Variables::START_AREA_VISIBILITY_TIME);
    EXPECT_FALSE(startingArea->isActivated());

    // Put the player in the starting area.
    player1.setX(2.5);
    player1.setY(2.5);

    // Pass enough time such that the player is in the starting area for long enough to activate.
    startingArea->updateTimers(players, Variables::START_GAME_TIME);
    EXPECT_TRUE(startingArea->isActivated());
}

/**
 * Verify that when a player is in the starting area for long enough that the StartingArea is deactivated
 * once this player leaves.
 */
TEST_F(StartingAreaTest, DeactivateLeavingPlayer) {
    list<Player *> players;
    Player player1 = Player(1);
    players.push_back(&player1);

    // The starting area becomes visible after this time.
    startingArea->updateTimers(players, Variables::START_AREA_VISIBILITY_TIME);
    EXPECT_FALSE(startingArea->isActivated());

    // Put the player in the starting area.
    player1.setX(2.5);
    player1.setY(2.5);

    // Pass enough time such that the player is in the starting area for long enough to activate.
    startingArea->updateTimers(players, Variables::START_GAME_TIME);
    EXPECT_TRUE(startingArea->isActivated());

    // Put the player outside of the starting area.
    player1.setX(10.0);
    player1.setY(10.0);

    // Update with the player that left the StartingArea.
    startingArea->updateTimers(players, 0.1);
    EXPECT_FALSE(startingArea->isActivated());
}

/**
 * Verify that when a player is in the starting area for long enough that the StartingArea is deactivated
 * once this player leaves.
 */
TEST_F(StartingAreaTest, DeactivateDisappearingPlayer) {
    list<Player *> players;
    Player player1 = Player(1);
    players.push_back(&player1);

    // The starting area becomes visible after this time.
    startingArea->updateTimers(players, Variables::START_AREA_VISIBILITY_TIME);
    EXPECT_FALSE(startingArea->isActivated());

    // Put the player in the starting area.
    player1.setX(2.5);
    player1.setY(2.5);

    // Pass enough time such that the player is in the starting area for long enough to activate.
    startingArea->updateTimers(players, Variables::START_GAME_TIME);
    EXPECT_TRUE(startingArea->isActivated());

    list<Player *> emptyList;

    // Update with the disappeared player.
    startingArea->updateTimers(emptyList, 0.1);
    EXPECT_FALSE(startingArea->isActivated());
}

/**
 * Verify that when a player is in the starting area for long enough that the StartingArea is activated.
 */
TEST_F(StartingAreaTest, ActivateMultiplePlayers) {
    list<Player *> players;
    Player player1 = Player(1);
    Player player2 = Player(2);
    players.push_back(&player1);
    players.push_back(&player2);

    // The starting area becomes visible after this time.
    startingArea->updateTimers(players, Variables::START_AREA_VISIBILITY_TIME);
    EXPECT_FALSE(startingArea->isActivated());

    // Put the players in the starting area.
    player1.setX(2.5);
    player1.setY(2.5);
    player2.setX(2.5);
    player2.setY(2.5);

    // Pass enough time such that the players are in the starting area for long enough to activate.
    startingArea->updateTimers(players, Variables::START_GAME_TIME);
    EXPECT_TRUE(startingArea->isActivated());

    list<Player *> players2;
    players2.push_back(&player2);

    // Update with the removed player1, it should remain active since player2 is still in the StartingArea.
    startingArea->updateTimers(players, 0.1);
    EXPECT_TRUE(startingArea->isActivated());
}