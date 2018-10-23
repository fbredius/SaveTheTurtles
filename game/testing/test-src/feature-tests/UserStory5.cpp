//
// Created by mariette on 20-6-18.
//

#include <gtest/gtest.h>
#include "../../../src/control/PlayerManager.h"
#include "../../../src/control/TurtleManager.h"
#include "../../../src/control/GameManager.h"
#include "../../../src/utility/CollisionHandler.h"
#include "../../../src/gameEntities/turtle/turtleStates/TurtleObjectiveState.h"

/**
 * Tests if the acceptance criteria for user story 5 are met.
 */
class UserStory5 : public ::testing::Test {
protected:
    virtual void SetUp() {
        testTurtleManager = new TurtleManager();
        testPlayerManager = new PlayerManager();
        testGameManager = new GameManager();
        testObstacleManager = new ObstacleManager();
        objectiveAreas = testGameManager->getObjectiveAreas();
        scaledRadius = Variables::scaleToPixels(Variables::OBJECTIVE_RADIUS);
    }

    TurtleManager *testTurtleManager;
    PlayerManager *testPlayerManager;
    GameManager *testGameManager;
    ObstacleManager *testObstacleManager;
    std::list<ObjectiveArea *> objectiveAreas;
    double scaledRadius;

    virtual void TearDown() {
        delete testTurtleManager;
        delete testPlayerManager;
        delete testGameManager;
        delete testObstacleManager;
    }
};

/**
 * Test if turtles move towards the objective area when pushed there
 */
TEST_F(UserStory5, Standard_Scenario_Test) {
    testTurtleManager->createTurtle();
    testPlayerManager->createPlayer(0);

    Player *player = testPlayerManager->getPlayer(0);
    Turtle *voldetort = testTurtleManager->getTurtles().front();

    /*
     * Place turtle and player close to each other.
     * Turtle is placed on the edge of the objective area.
     */
    player->setX(sqrt(scaledRadius));
    player->setY(sqrt(scaledRadius));

    voldetort->setInteractable();
    voldetort->setX(sqrt(scaledRadius) + sqrt(Variables::scaleToPixels(Variables::PLAYER_FORCE_RADIUS)));
    voldetort->setY(sqrt(scaledRadius) + sqrt(Variables::scaleToPixels(Variables::PLAYER_FORCE_RADIUS)));

    CollisionHandler *collisionHandler = new CollisionHandler(testPlayerManager, testTurtleManager,
                                                              testObstacleManager, objectiveAreas);

    int temp = Variables::getScore();

    //Check if collision is detected.
    collisionHandler->handleCollision();

    InputData inputData(InputData::mouse);
    std::list<InputData *> inputList;
    inputList.push_back(&inputData);

    //Update the turtle
    voldetort->update(inputList);
    voldetort->update(inputList);
    voldetort->update(inputList);
    voldetort->update(inputList);
    voldetort->update(inputList);

    //Sets the turtle to the objective state, if in the right area.
    collisionHandler->handleCollision();

    //Assert if the turtle goes into objective state.
    ASSERT_TRUE(dynamic_cast<TurtleObjectiveState *>(voldetort->getCurrentState()));

    //Assert if the score is updated.
    ASSERT_EQ(Variables::getScore() - temp, 1);
}
