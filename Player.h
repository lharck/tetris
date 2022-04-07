#include "Arduino.h" 
#include "Piece.h"

class Player {
private:
    int score;    
    static const int JOY_X_PIN = A0, JOY_Y_PIN = A1, JOY_CLICK_PIN = 13;
    static const int LEFT_ROTATE_PIN = 6, RIGHT_ROTATE_PIN = 7, HARD_DROP_PIN = 5;
    
    static const int NUM_BUTTONS = 4;
    const byte buttonPins[NUM_BUTTONS] = {HARD_DROP_PIN, LEFT_ROTATE_PIN, RIGHT_ROTATE_PIN, JOY_CLICK_PIN};

    unsigned long lastButtonPress = millis(); 

    byte previousButtonStates[NUM_BUTTONS] = {1,1,1,1};
    byte currentButtonStates[NUM_BUTTONS] = {1,1,1,1};

    void processJoystick();
    void processButtons(); 

public:
    Piece* piece;
    Player();

    void onEliminiated(); // TODO: ??? 
    void onDropButtonPressed();
    void onJoystickPressed();
    void onLeftRotatePressed();
    void onRightRotatePressed();
    void onJoystickMoved(int x, int y);
    void checkInputChange();
    void getNewPiece(const int NUM_COLS);
};
