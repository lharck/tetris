#include "Arduino.h"

class Game;
class Piece {
   private:
    byte (*pBoard)[32][8];
    void createArray();
    // Defines the width / height of each tetris piece
    static const byte PIECE_DIMENSIONS[7][2];
    static const byte PIECE_BOARD_COORDINATES[7][4];
    static const byte PIECE_ARRAY_COORDINATES[7][4];
    static const int MAX_X_SIZE=4, MAX_Y_SIZE=4;

    /* The definitions for each tetris piece
    Based Tetris pieces on the following image:
    https://static.wikia.nocookie.net/tetrisconcept/images/3/3d/SRS-pieces.png/revision/latest?cb=20060626173148
    Guide to piece names:
    https://qph.fs.quoracdn.net/main-qimg-bf707b034d64c955a8f5c1f89d7000aa
    */
    static const byte PIECE_TEMPLATES[7][MAX_X_SIZE][MAX_Y_SIZE];
    byte type;
    byte copy[MAX_X_SIZE][MAX_Y_SIZE];

    void rotate90C();
    void rotate90CC();
    void copyArray();
   
   public:
    signed char xBoardLeft, xBoardRight, yBoardUp, yBoardLow, xArrayLeft, xArrayRight, yArrayUp, yArrayLow;
    byte width, height;
    byte pieceArray[MAX_X_SIZE][MAX_Y_SIZE];

    signed char x, y;
  
    Piece(int PIECE_TYPE, byte (*pBoard)[32][8]);
    
    bool hasFallen();
    bool isColliding();
    
    void rotate(String direction);
    void move(int xDirection, int yDirection);
    void hardDrop();
};
