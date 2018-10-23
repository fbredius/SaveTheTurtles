//
// Created by timbuntu on 8-5-18.
//

#ifndef GAME_GAMEMANAGER_H
#define GAME_GAMEMANAGER_H

#include "../utility/BasicState.h"
#include "PlayerManager.h"
#include "../gameEntities/ObjectiveArea.h"
#include <list>

/**
 * GameManager that handles the entire game.
 * It is responsible for maintaining a PlayerManager, TurtleManager, 
 * ObstacleManager and a set of ObjectiveAreas.
 * Its behaviour changes based on its currentState.
 * Update and render calls are propagated to the managers and the state.
 */
class GameManager {

private:
    BasicState *currentState;
    std::list<ObjectiveArea *> objectiveAreas;

    void createObjectiveAreas();

public:
    GameManager();

    void update(std::list<InputData *> inputList);

    void render(RenderWrapper *renderWrapper);

    BasicState *getCurrentState() const;

    void setCurrentState(BasicState *currentState);

    PlayerManager *getPlayerManager() const;

    list<ObjectiveArea *> getObjectiveAreas() const;

};

#endif //GAME_GAMEMANAGER_H
