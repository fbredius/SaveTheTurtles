//
// Created by maxim on 17-5-18.
//

#ifndef GAME_INPUTDATA_H
#define GAME_INPUTDATA_H

/**
 * The inputData object, the object that containerizes the game input.
 * This allows different frameworks to provide input for the game logic.
 * While running in standalone, mouse input is converted to InputData objects.
 * While running with IMOVE, tracked persons are converted to InputData.
 */
class InputData {

public:
    enum inputType {
        mouse = 0, arrowkeys = 1, wasdkeys = 2, imove = 3
    };

private:
    bool valid;
    float frameTime;
    double playerX;
    double playerY;
    bool spacePressed;
    bool kPressed;
    bool lPressed;
    bool jPressed;
    int id;
    InputData::inputType type;

public:
    explicit InputData(inputType type);

    // Getters
    bool isValid() const;

    int getId() const;

    float getFrameTime() const;

    double getPlayerX() const;

    double getPlayerY() const;

    bool isSpacePressed() const;

    bool isKPressed() const;

    bool isLPressed() const;

    bool isJPressed() const;

    // Setters
    void setValid();

    void setId(int id);

    void setFrameTime(float frameTime);

    void setPlayerX(double playerX);

    void setPlayerY(double playerY);

    InputData::inputType getType();

    void pressSpace();

    void pressK();

    void pressL();

    void pressJ();

};

#endif //GAME_INPUTDATA_H