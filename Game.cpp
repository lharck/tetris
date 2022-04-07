#include "Arduino.h" 
#include "Game.h"

void printBinary(byte inByte){
  for (int b = 7; b >= 0; b--){
    Serial.print(bitRead(inByte, b));
  }
  Serial.println();
}

Game::Game(){
  ledMatrix = new LedControl(DIN,CLK,CS,4);
  
  for(int i = 0; i < 4; i++){
    ledMatrix->shutdown(i,false); // turn off power saving, enables display
    ledMatrix->setIntensity(i,1); // sets brightness (0~15 possible values)
    // ledMatrix->clearDisplay(i); // clear screen
  }
  
  initBoard();
  player = new Player();
}

// Populate board array with zeros
void Game::initBoard(){ 
    for(byte i = 0; i < ROWS; i++){
        for(byte j = 0; j < COLS; j++){
            board[i][j] = 0;
        }
    }
}

// Converts the 2d tetris board to an array of bytes  
byte* Game::serialize(byte (&boardWithPiece)[ROWS][COLS]){
    static byte serializedBoard[ROWS];

    for(int i = 0; i < ROWS; i++){
        byte result = 0;

        for(int j = 0; j < COLS; j++){
            result += ((byte)(boardWithPiece[i][j]) << (7-j));
        }

        serializedBoard[i] = result; 
    }

    return serializedBoard;
}

void Game::start(){
  player->getNewPiece(COLS);
  updateBoard();
}

// Copies board and piece into into result array:
void Game::cloneBoardWithPiece(byte (&boardWithPiece)[ROWS][COLS]){
    // Copies board into boardWithPiece array
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            boardWithPiece[i][j] = board[i][j];
        }
    }

    // Copies piece onto boardWithPiece array
    // Starting at it's current position
    for(int i = 0; i < player->piece->height; i++){
      for(int j = 0; j < player->piece->width; j++){
        int xOff = player->piece->x+j;
        int yOff = player->piece->y+i;
        boardWithPiece[yOff][xOff] = player->piece->pieceArray[i][j];
      }
    }
}

// Draws all the blocks on the tetris board on the led display
void Game::updateBoard(){
    byte boardWithPiece[ROWS][COLS];
    cloneBoardWithPiece(boardWithPiece);
    
    byte* blocksToDisplay = serialize(boardWithPiece);

    for(int i = 0; i < 32; i++){
      int row = (i%8);
      int address = 3-(i/8);
      ledMatrix->setColumn(address, row, blocksToDisplay[i]);
    }
}
