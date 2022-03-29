#include "Player.h"
#include "LedControl.h"


class Game {
private:
    static const byte DIN=12, CLK=11, CS=10; // pins for the led matrix
    LedControl* ledMatrix;

	const static byte ROWS = 32;
	const static byte COLS = 8;
	// String state;
	byte board[ROWS][COLS]; 

    Player* player;
    
	void initBoard();
    byte* serialize(byte (&boardWithPiece)[ROWS][COLS]);
    void cloneBoardWithPiece(byte (&boardWithPiece)[ROWS][COLS]);
    void displayBlocks();

public:
    Game();
    void drawBoard();
    void start();

    /*
        TODO: James
        - if it is a true game over, calculates / displays winner
        - cleans up pieces and game class
        - if the game was interrupted, just clean up game class and skip clean up
    */
    void stop();

    /*
        TODO: James:
        When a piece is placed, clears the rows that have been completed (if any)
        And then drops the pieces down (if any) (maybe could be another function)
        calculate score (maybe could be another function)
    */
    void clearRows();

    void recieveGarbage(); // TODO: James
    void sendGarbage(); // TODO: James
    void calculateGarbage(); // TODO: James
};