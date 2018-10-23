//
// Created by timbuntu on 8-5-18.
//

#ifndef GAME_BASICSTATE_H
#define GAME_BASICSTATE_H

#include "../storage/InputData.h"
#include "../rendering/RenderWrapper.h"
#include <list>

/**
 * Defines the basic state class. Every state needs to be able to update and render.
 */

class BasicState {

public:
    virtual ~BasicState() = default;

    virtual void update(std::list<InputData *> inputList) = 0;

    virtual void render(RenderWrapper *renderWrapper) = 0;

    virtual std::string getStateIdentifier() = 0;
};

#endif //GAME_BASICSTATE_H