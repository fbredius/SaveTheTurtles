//
// Created by thijmen on 30-5-18.
//

#include <gtest/gtest.h>
#include "../../../../src/utility/CollisionHandler.h"
#include "../../../../src/storage/Variables.h"
#include "../../../../src/utility/MathUtilities.h"

/**
 * Test class for the CollisionHandler class.
 */
class CollisionHandlerTest : public ::testing::Test {
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
    std::list<Obstacle *> *obstacles;
    std::list<ObjectiveArea *> *objectiveAreas;

    virtual void TearDown() {
        delete testTurtleManager;
        delete testPlayerManager;
        delete testObstacleManager;
    }
};

/**
 * Test collision method for two turtles in the spawn.
 */
TEST_F(CollisionHandlerTest, TwoTurtlesCollisionTest) {

    testTurtleManager->createTurtle();
    testTurtleManager->createTurtle();

    CollisionHandler *collisionHandler = new CollisionHandler(testPlayerManager, testTurtleManager,
                                                              testObstacleManager, *objectiveAreas);

    collisionHandler->handleCollision();

    Turtle *turtle = testTurtleManager->getTurtles().front();

    // No action is implemented so nothing should happen.
    EXPECT_FALSE(turtle->isColliding());
}

/**
 * Test collision method copy constructor.
 */
TEST_F(CollisionHandlerTest, CopyConstructorTest) {

    CollisionHandler *collisionHandler = new CollisionHandler(testPlayerManager, testTurtleManager,
                                                              testObstacleManager, *objectiveAreas);

    auto *copyHandler = new CollisionHandler(*collisionHandler);
    auto *testTurtle1 = new Turtle();

    EXPECT_EQ(copyHandler->checkTurtleCollisions(testTurtle1), false);
}

/**
 * Test collision method for a player and interactable turtle.
 */
TEST_F(CollisionHandlerTest, PlayerTurtleCollisionInsideTest) {

    std::list<Player *> players = testPlayerManager->getPlayers();
    auto *testTurtle1 = new Turtle();
    auto *testPlayer = new Player(0);

    testTurtle1->setInteractable();
    testTurtle1->setX(1.0);
    testTurtle1->setY(1.0);
    testPlayer->setX(1.0);
    testPlayer->setY(1.0);

    players.push_back(testPlayer);
    testPlayerManager->setPlayers(players);

    CollisionHandler *collisionHandler = new CollisionHandler(testPlayerManager, testTurtleManager,
                                                              testObstacleManager, *objectiveAreas);

    EXPECT_EQ(collisionHandler->checkPlayerCollisions(testTurtle1), true);
}

/**
 * Test collision method for a player and interactable turtle.
 */
TEST_F(CollisionHandlerTest, PlayerTurtleCollisionOutsideTest) {

    std::list<Player *> players = testPlayerManager->getPlayers();
    auto *testTurtle1 = new Turtle();
    auto *testPlayer = new Player(0);

    testTurtle1->setInteractable();
    testTurtle1->setX(200.0 + Variables::scaleToPixels(Variables::PLAYER_FORCE_RADIUS) - 1);
    testTurtle1->setY(200.0);
    testPlayer->setX(200.0);
    testPlayer->setY(200.0);

    players.push_back(testPlayer);
    testPlayerManager->setPlayers(players);

    CollisionHandler *collisionHandler = new CollisionHandler(testPlayerManager, testTurtleManager,
                                                              testObstacleManager, *objectiveAreas);

    EXPECT_EQ(collisionHandler->checkPlayerCollisions(testTurtle1), true);
}

/**
 * Test collision method for a player and interactable turtle.
 */
TEST_F(CollisionHandlerTest, PlayerTurtleNoCollisionTest) {

    std::list<Player *> players = testPlayerManager->getPlayers();
    auto *testTurtle1 = new Turtle();
    auto *testPlayer = new Player(0);

    testTurtle1->setInteractable();
    testTurtle1->setX(200.0 + Variables::scaleToPixels(Variables::PLAYER_FORCE_RADIUS) - 1);
    testTurtle1->setY(200.0 + Variables::scaleToPixels(Variables::PLAYER_FORCE_RADIUS) - 1);
    testPlayer->setX(200.0);
    testPlayer->setY(200.0);

    players.push_back(testPlayer);
    testPlayerManager->setPlayers(players);

    CollisionHandler *collisionHandler = new CollisionHandler(testPlayerManager, testTurtleManager,
                                                              testObstacleManager, *objectiveAreas);

    EXPECT_EQ(collisionHandler->checkTurtleCollisions(testTurtle1), false);
}

/**
 * Test collision method for a player and turtle outside of objective.
 */
TEST_F(CollisionHandlerTest, PlayerTurtleCollisionTest2) {

    std::list<Player *> players = testPlayerManager->getPlayers();
    auto *testTurtle1 = new Turtle();
    auto *testPlayer = new Player(0);

    testTurtle1->setInteractable();
    testTurtle1->setInObjective(false);

    testTurtle1->setX(Variables::PIXEL_WIDTH / 2.0);
    testTurtle1->setY(Variables::PIXEL_HEIGHT / 2.0);
    testPlayer->setX(Variables::PIXEL_WIDTH / 2.0);
    testPlayer->setY(Variables::PIXEL_HEIGHT / 2.0);

    players.push_back(testPlayer);
    testPlayerManager->setPlayers(players);

    CollisionHandler *collisionHandler = new CollisionHandler(testPlayerManager, testTurtleManager,
                                                              testObstacleManager, *objectiveAreas);

    EXPECT_EQ(collisionHandler->checkPlayerCollisions(testTurtle1), true);
}

/**
 * Test collision method for a player and non-interactable turtle.
 */
TEST_F(CollisionHandlerTest, PlayerTurtleCollisionTest3) {

    testTurtleManager->createTurtle();
    testPlayerManager->createPlayer();

    Turtle turtle = Turtle();

    CollisionHandler *collisionHandler = new CollisionHandler(testPlayerManager, testTurtleManager,
                                                              testObstacleManager, *objectiveAreas);

    EXPECT_EQ(collisionHandler->checkPlayerCollisions(&turtle), false);
}

/**
 * Test collision method for two colliding turtles.
 */
TEST_F(CollisionHandlerTest, TwoTurtlesCollidingTest) {

    auto *testTurtle1 = new Turtle();
    auto *testTurtle2 = new Turtle();

    testTurtle1->setX(1.0);
    testTurtle1->setY(1.0);
    testTurtle2->setX(1.0);
    testTurtle2->setY(1.0);

    std::list<Turtle *> turtles = testTurtleManager->getTurtles();
    turtles.push_back(testTurtle1);
    testTurtleManager->setTurtles(turtles);

    CollisionHandler *collisionHandler = new CollisionHandler(testPlayerManager, testTurtleManager,
                                                              testObstacleManager, *objectiveAreas);

    EXPECT_EQ(collisionHandler->checkTurtleCollisions(testTurtle2), true);
}

/**
 * Test collision method for an obstacle and interactable turtle.
 */
TEST_F(CollisionHandlerTest, CheckCollisionTOTest) {
    auto *testTurtle1 = new Turtle();
    testTurtle1->setX(200.0);
    testTurtle1->setY(200.0);

    auto *obstacle = new Obstacle(200.0, 200.0);
    auto *obstacleList = new list<Obstacle *>;
    obstacleList->push_back(obstacle);

    auto *obstacleManager = new ObstacleManager();
    obstacleManager->setObstacles(obstacleList);

    CollisionHandler *collisionHandler = new CollisionHandler(testPlayerManager, testTurtleManager,
                                                              obstacleManager, *objectiveAreas);

    EXPECT_TRUE(collisionHandler->checkObstacleCollisions(testTurtle1));
}