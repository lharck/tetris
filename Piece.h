#include "Arduino.h" 

class Piece {
private:


    void createArray();

public:

    // Defines the width / height of each tetris piece
    static const byte PIECE_DIMENSIONS[7][2];
        
    /* The definitions for each tetris piece
    Based Tetris pieces on the following image: https://static.wikia.nocookie.net/tetrisconcept/images/3/3d/SRS-pieces.png/revision/latest?cb=20060626173148
    Guide to piece names: https://qph.fs.quoracdn.net/main-qimg-bf707b034d64c955a8f5c1f89d7000aa
    */
    static const byte PIECE_TEMPLATES[7][4][4];
    
    byte type;
	byte** pieceArray;
    byte width;
    byte height; 
    byte x;
    byte y;
        
    Piece(const int PIECE_TYPE, const int NUM_COLS);
    void printArray();
    // void rotate(String direction); // TODO: MATT
    void move(int xDirection, int yDirection); // TODO: Lucy
    // void drop(byte** grid); // TODO: ???
};

