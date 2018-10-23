//
// Created by mariette on 14-6-18.
//

#include <gtest/gtest.h>
#include "../../../src/control/PlayerManager.h"
#include "../../../src/control/TurtleManager.h"
#include "../../../src/control/GameManager.h"
#include "../../../src/utility/CollisionHandler.h"
#include "../../../src/gameEntities/turtle/turtleStates/TurtleState.h"

/**
 * Tests if the acceptance criteria for user story 1 are met.
 */
class UserStory1 : public ::testing::Test {
protected:
    virtual void SetUp() {
        testTurtleManager = new TurtleManager();
        testPlayerManager = new PlayerManager();
        testObstacleManager = new ObstacleManager();
        objectiveAreas = new std::list<ObjectiveArea *>;
    }

    TurtleManager *testTurtleManager;
    PlayerManager *testPlayerManager;
    ObstacleManager *testObstacleManager;
    std::list<ObjectiveArea *> *objectiveAreas;

    virtual void TearDown() {
        delete testTurtleManager;
        delete testPlayerManager;
        delete testObstacleManager;
        delete objectiveAreas;
    }
};

/**
 * Test if turtles move when a player collides with them.
 */
TEST_F(UserStory1, Standard_Scenario_Test) {

    // Disable the border offset to prevent unwanted interference with turtle movement in this small simulation.
    double temp = Variables::TURTLE_BORDER_OFFSET;
    Variables::TURTLE_BORDER_OFFSET = 0.0;

    //Place turtle and player close to each other.
    auto *player = new Player(1);
    player->setX(9.8);
    player->setY(9.8);

    auto *voldetort = new Turtle();
    voldetort->setInteractable();
    voldetort->setX(10 + Variables::scaleToPixels(Variables::PLAYER_FORCE_RADIUS) - 1);
    voldetort->setY(10);

    double x = voldetort->getX();
    double y = voldetort->getY();
    double dir = voldetort->getDirection();

    InputData inputData(InputData::mouse);
    inputData.setFrameTime(0.1);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);

    list<Player *> collidedPlayers;
    collidedPlayers.push_back(player);

    //Update the turtle
    auto *state1 = dynamic_cast<TurtleState *>(voldetort->getCurrentState());
    state1->onCollide(collidedPlayers, false);
    voldetort->update(inputList);
    voldetort->update(inputList);
    voldetort->update(inputList);
    voldetort->update(inputList);
    voldetort->update(inputList);

    //Check if the turtle has moved
    double loc_diff = abs(x - voldetort->getX()) + abs(y - voldetort->getY());
    EXPECT_NE(loc_diff, 0);

    double dir_diff = dir - voldetort->getDirection();
    EXPECT_NE(dir_diff, 0);

    // Restore to original value.
    Variables::TURTLE_BORDER_OFFSET = temp;
}

/**
 * Tests if a turtle rushes back once they reach the edge of the playing field.
 */
TEST_F(UserStory1, Edge_Case_Scenario) {
    testTurtleManager->createTurtle();
    testPlayerManager->createPlayer(0);

    Player *player = testPlayerManager->getPlayer(0);
    Turtle *voldetort = testTurtleManager->getTurtles().front();

    /*
     * Place turtle and player close to each other.
     * Turtle is placed at the boundary.
     */
    voldetort->setX(0);
    voldetort->setY(500);

    voldetort->setInteractable();
    player->setX(Variables::scaleToPixels(Variables::PLAYER_FORCE_RADIUS) - 2);
    player->setY(500);

    CollisionHandler *collisionHandler = new CollisionHandler(testPlayerManager, testTurtleManager,
                                                              testObstacleManager, *objectiveAreas);

    //Check if the turtle and player collide.
    collisionHandler->handleCollision();

    double x = voldetort->getX();
    double y = voldetort->getY();

    InputData inputData(InputData::mouse);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);
    inputData.setFrameTime(0.01);

    voldetort->update(inputList);
    voldetort->update(inputList);
    voldetort->update(inputList);
    voldetort->update(inputList);
    voldetort->update(inputList);

    //Check if the turtle is moving closer to the spawn area.
    ASSERT_TRUE(voldetort->getX() - x >= 0);
    ASSERT_TRUE(y - voldetort->getY() <= 10);
}