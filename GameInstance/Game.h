#include "LedControl.h"
#include "Player.h"

class Game {
   private:
    static const byte DIN = 12, CS = 11, CLK = 10;  // pins for the led matrix
    LedControl* ledMatrix;

    const static byte ROWS = 32;
    const static byte COLS = 8;

    byte isFirstFrame = 1;
    byte prevDisplayedBlocks[ROWS];

    void initBoard();
    void serialize(byte (&boardWithPiece)[ROWS][COLS],
                   byte (&blocksToDisplay)[ROWS]);
    void copyBoard(byte (&boardWithPiece)[ROWS][COLS]);
    int linesCleared;
   public:
    Game();

    enum class States {
        GAME_IN_PROGRESS,
        WAITING_FOR_GAME,
    };

    byte board[ROWS][COLS];

    States state;
    String commandToSend;

    Player* player;
    void drawBoard();
    void start();
    void reset();
    void updateBoard();
    void placePiece(byte (&boardToEdit)[ROWS][COLS]);

    /*
        TODO: James
        - if it is a true game over, calculates / displays winner
        - cleans up pieces and game class
        - if the game was interrupted, just clean up game class and skip clean
        - sets game state to game over
       up
    */
    void deleteLine(int lineToDelete);
    void deleteClearedLines();
    void recieveGarbage(int numLines);
    void sendGarbage();
};
