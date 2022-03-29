#include "Arduino.h" 
#include "Piece.h"

class Player {
private:
    // int score;    
    // int** board;

public:
    Piece* piece;
    Player();

    // Function called when the player’s tetris pieces reach the top of the screen
    // Set players state to eliminated
    void onEliminiated(); // TODO: ??? 

    void onLeftRotatePressed(); // TODO: LUCY calls piece->rotate("Left")
    void onRightRotatePressed(); // TODO: LUCY calls piece->rotate("Right")
    void onJoystickMoved(int x, int y); // TODO: LUCY rotates piece based on direction of joystick
    void checkInputChanged(); // TODO: LUCY calls one of the functions above based on how the input changes
    void getNewPiece(const int NUM_COLS);
};