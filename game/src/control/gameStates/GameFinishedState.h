//
// Created by maxim on 17-5-18.
//

#ifndef GAME_GAMEFINISHEDSTATE_H
#define GAME_GAMEFINISHEDSTATE_H

#include "../TurtleManager.h"
#include "../PlayerManager.h"
#include "../GameManager.h"

/**
 * The finished state, after the game has ended.
 * This state renders a specific background depending on how the player did
 * and it shows the score.
 */
class GameFinishedState : public BasicState {
private:
    GameManager *gameManager;

    bool fadeFinished;

    double duration;

    void resetGame();

public:
    bool hasFinished;

    explicit GameFinishedState(GameManager *gameManager);

    void update(std::list<InputData *> inputList) override;

    void render(RenderWrapper *renderWrapper) override;

    std::string getStateIdentifier() override;
};


#endif //GAME_GAMEFINISHEDSTATE_H
