#include "Arduino.h" 

class Game;
class Piece {
private:
    void createArray();

public:

    // Defines the width / height of each tetris piece
    static const byte PIECE_DIMENSIONS[7][2];
    static const int MAX_X_SIZE=4, MAX_Y_SIZE=4;
    /* The definitions for each tetris piece
    Based Tetris pieces on the following image: https://static.wikia.nocookie.net/tetrisconcept/images/3/3d/SRS-pieces.png/revision/latest?cb=20060626173148
    Guide to piece names: https://qph.fs.quoracdn.net/main-qimg-bf707b034d64c955a8f5c1f89d7000aa
    */
    static const byte PIECE_TEMPLATES[7][MAX_X_SIZE][MAX_Y_SIZE];

    byte type;
	byte pieceArray[MAX_X_SIZE][MAX_Y_SIZE];
    byte width, height; 
    byte x, y;
        
    Piece(int PIECE_TYPE, const int NUM_COLS);
    void printArray();
    void rotate(String direction); // TODO: MATT
    void move(int xDirection, int yDirection);
    // void drop(byte** grid); // TODO: ???
};
