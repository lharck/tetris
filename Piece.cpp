#include "Arduino.h" 
#include "Piece.h" 
   
const byte Piece::PIECE_DIMENSIONS[7][2] = {{4,4}, {3,3}, {3,3}, {3,4}, {3,3}, {3,3}, {3,3}};

const byte Piece::PIECE_TEMPLATES[7][4][4] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, // I piece
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // L piece
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // J piece
    {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, // O piece
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // S piece
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // T piece
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, // Z piece
};

// Creates a 2d array representing the tetris piece
void Piece::createArray(){
    pieceArray = new byte*[height];

    for(int i = 0; i < height; i++)
        pieceArray[i] = new byte[width];

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            pieceArray[i][j] = PIECE_TEMPLATES[type][i][j];
        }
    }
}

Piece::Piece(const int PIECE_TYPE, const int NUM_COLS){
    type = PIECE_TYPE;
    width = PIECE_DIMENSIONS[0][1];
    height = PIECE_DIMENSIONS[0][0];

    int middleOfRow = (NUM_COLS-width)/2;
    Serial.print((middleOfRow));

    x = middleOfRow;
    y = 0;

	this->createArray();
}

// void Piece::rotate(String direction){

// }

/*
    Moves the piece in the x direction on a grid
    - if x is 1 => moves left,  if x is -1 => moves right
    - if y is -1 => moves down
    - if y is 1 => not valid throw error
*/
void Piece::move(int xDirection, int yDirection){
    // TODO: make sure xDirection is either 1/-1/0
    // TODO: make sure yDirection is either 0/1

    x += xDirection;
    y += yDirection;
}

// void Piece::drop(byte** grid){

// }

