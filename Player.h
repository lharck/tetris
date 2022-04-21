#include "Arduino.h"
#include "Piece.h"

class Player {
   private:
    byte (*pBoard)[32][8];
    static const int JOY_X_PIN = A0, JOY_Y_PIN = A1, JOY_CLICK_PIN = 13;
    static const int LEFT_ROTATE_PIN = 6, RIGHT_ROTATE_PIN = 7,
                     HARD_DROP_PIN = 5;

    static const int NUM_BUTTONS = 4;
    const byte buttonPins[NUM_BUTTONS] = {HARD_DROP_PIN, LEFT_ROTATE_PIN,
                                          RIGHT_ROTATE_PIN, JOY_CLICK_PIN};

    unsigned long lastButtonPressTime;
    unsigned long lastSoftDropTime;
    unsigned long verticalHoldStartTime;
    
    String prevJoystickDirection;

    byte previousButtonStates[NUM_BUTTONS] = {1, 1, 1, 1};
    byte currentButtonStates[NUM_BUTTONS] = {1, 1, 1, 1};

    String processJoystick();
    String processButtons();

   public:
    Piece* piece;
    Player(byte (*pBoard)[32][8]);
    int score;

    void onEliminiated();  // TODO: ???
    void onDropButtonPressed();
    void onJoystickPressed();
    void onLeftRotatePressed();
    void onRightRotatePressed();
    String onJoystickMoved(int x, int y);
    String checkInputChange();
    void getNewPiece();
};
