//
// Created by mariette on 1-6-18.
//

#include "gtest/gtest.h"
#include "../../../../src/gameEntities/turtle/Turtle.h"
#include "../../../../src/gameEntities/turtle/turtleStates/TurtleSpawnState.h"
#include "../../../../src/storage/Variables.h"
#include "../../../../../../src/gameEntities/turtle/turtleStates/TurtleIdleState.h"
#include "../../../../../../src/gameEntities/turtle/turtleStates/TurtleObjectiveState.h"
#include <cmath>

/**
 * Testing class for the Turtle Spawn state.
 */
class TurtleSpawnStateTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        Variables::ID_COUNTER = 0;
        voldetort = Turtle();
        voldespawn = new TurtleSpawnState(&voldetort);
        voldetort.setCurrentState(voldespawn);
    }

    Turtle voldetort = Turtle();
    TurtleSpawnState *voldespawn = new TurtleSpawnState(&voldetort);
};

/**
 * Tests if the getter for alpha works properly.
 */
TEST_F(TurtleSpawnStateTest, alphaGetterTest) {
    EXPECT_DOUBLE_EQ(0.0, (*voldespawn).getAlpha());
}

/**
 * Tests if the getter for timer works properly.
 */
TEST_F(TurtleSpawnStateTest, timerGetterTest) {
    EXPECT_DOUBLE_EQ(Variables::EGG_SPAWN_TIME, (*voldespawn).getTimer());
}

/**
 * Tests if the update function works properly in most cases.
 */
TEST_F(TurtleSpawnStateTest, updateTest) {
    InputData inputData(InputData::mouse);
    inputData.setFrameTime(0.01);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);
    double temp = (*voldespawn).getTimer();

    // Tests if voldetort is not interactable.
    EXPECT_FALSE(voldetort.isInteractable());
    (*voldespawn).update(inputList);

    // Tests if alpha and timer are update properly.
    EXPECT_DOUBLE_EQ(255.0 - temp / Variables::EGG_SPAWN_TIME * 255.0, (*voldespawn).getAlpha());
    EXPECT_DOUBLE_EQ((*voldespawn).getTimer(), temp - inputList.front()->getFrameTime());

    // Tests if voldetort becomes interactable when the timer runs out.
    while ((*voldespawn).getTimer() >= 0) {
        (*voldespawn).update(inputList);
    }
    (*voldespawn).update(inputList);
    EXPECT_TRUE(voldetort.isInteractable());
}

/**
 * Verify that this state returns the correct StateIdentifier when called from a BasicState instance.
 */
TEST_F(TurtleSpawnStateTest, StateIdentifierTest) {
    voldetort.setCurrentState(new TurtleSpawnState(&voldetort));
    EXPECT_EQ(voldetort.getCurrentState()->getStateIdentifier(), "1: TurtleSpawnState");
}

/**
 * Verify that nothing happens with regard to state switching
 * when a turtle collides with another turtle while in the TurtleSpawnState.
 */
TEST_F(TurtleSpawnStateTest, onCollideTurtle) {
    voldetort.setCurrentState(new TurtleSpawnState(&voldetort));
    Turtle turtle = Turtle();
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    state->onCollide(turtle);

    EXPECT_TRUE(dynamic_cast<TurtleSpawnState *>(voldetort.getCurrentState()));
}

/**
 * Verify that a turtle that collides with a player while in the TurtleSpawnState hatches
 * if the collision is with the outer hitBox of the player.
 */
TEST_F(TurtleSpawnStateTest, onCollidePlayerHatch) {
    voldetort.setCurrentState(new TurtleSpawnState(&voldetort));
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());

    Player player = Player(1);
    list<Player *> collidedPlayers;
    collidedPlayers.push_back(&player);
    state->onCollide(collidedPlayers, false);

    EXPECT_TRUE(dynamic_cast<TurtleIdleState *>(voldetort.getCurrentState()));
}

/**
 * Verify that a turtle that collides with an ObjectiveArea while in the TurtleSpawnState goes into
 * the TurtleObjectiveState.
 * And verify that the score is incremented.
 */
TEST_F(TurtleSpawnStateTest, onCollideObjectiveArea) {
    voldetort.setCurrentState(new TurtleSpawnState(&voldetort));
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());
    ObjectiveArea objectiveArea = ObjectiveArea(1, 5, 1, 5);
    int score = Variables::getScore();

    state->onCollide(objectiveArea);

    EXPECT_TRUE(dynamic_cast<TurtleObjectiveState *>(voldetort.getCurrentState()));
    EXPECT_EQ(Variables::getScore(), score + 1);
    EXPECT_FALSE(voldetort.isAlive());
    Variables::setScore(score);
}

/**
 * Verify that a turtle that collides with an obstacle while in the TurtleSpawnState dies.
 */
TEST_F(TurtleSpawnStateTest, onCollideObstacle) {
    voldetort.setCurrentState(new TurtleSpawnState(&voldetort));
    auto *state = dynamic_cast<TurtleState *>(voldetort.getCurrentState());

    state->onCollideObstacle();

    EXPECT_FALSE(voldetort.isAlive());
}