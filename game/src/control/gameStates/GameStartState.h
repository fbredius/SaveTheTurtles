//
// Created by maxim on 17-6-18.
//

#ifndef GAME_GAMESTARTSTATE_H
#define GAME_GAMESTARTSTATE_H


#include "../GameManager.h"
#include "../TurtleManager.h"

/**
 * The starting state, when the game is displaying the tutorial.
 */
class GameStartState : public BasicState {
private:
    GameManager *gameManager;
    PlayerManager *playerManager;
    double timer;

    void runGame();

public:
    GameStartState(GameManager *gameManager, PlayerManager *playerManager);

    void update(std::list<InputData *> inputList) override;

    void render(RenderWrapper *renderWrapper) override;

    std::string getStateIdentifier() override;
};


#endif //GAME_GAMESTARTSTATE_H
