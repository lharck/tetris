#include "Game.h"

#include "Arduino.h"

void printBinary(byte inByte) {
    for (int b = 7; b >= 0; b--) {
        Serial.print(bitRead(inByte, b));
    }
    Serial.println();
}

Game::Game() {
    ledMatrix = new LedControl(DIN, CLK, CS, 4);
    player = new Player(&board);
    
    this->reset();
}

// Populate board array with zeros
void Game::initBoard() {
    for (byte i = 0; i < ROWS; i++) {
        for (byte j = 0; j < COLS; j++) {
            board[i][j] = 0;
        }
    }
}

// Converts the 2d tetris board to an array of bytes
void Game::serialize(byte (&boardWithPiece)[ROWS][COLS],
                     byte (&serializedBoard)[ROWS]) {
    for (int i = 0; i < ROWS; i++) {
        byte result = 0;

        for (int j = 0; j < COLS; j++) {
            result += ((byte)(boardWithPiece[i][j]) << j);
        }

        serializedBoard[i] = result;
    }
}

void Game::start() {
  state = States::GAME_IN_PROGRESS;
  player->getNewPiece();
  updateBoard();
}

void Game::reset(){
  state = States::WAITING_FOR_GAME;
  player->reset();

  isFirstFrame = 1;
  linesCleared = 0;
  
  initBoard();
  
  for(int i = 0; i < ROWS; i++)
    prevDisplayedBlocks[i] = B00000000;
    
  commandToSend = "";

  for (int i = 0; i < 4; i++) {
      ledMatrix->shutdown(i, false);
      ledMatrix->setIntensity(i, 1);
      //ledMatrix->clearDisplay(i);
  }
}

// Copies board into into boardWithPiece array:
void Game::copyBoard(byte (&boardWithPiece)[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            boardWithPiece[i][j] = board[i][j];
        }
    }
}

// places piece onto board
void Game::placePiece(byte (&boardToEdit)[ROWS][COLS]){
  for (int i = 0; i < player->piece->height; i++) {
    for (int j = 0; j < player->piece->width; j++) {
      int xOff = player->piece->x + j;
      int yOff = player->piece->y + i;
      
      byte block = player->piece->pieceArray[i][j];
      
      if(block == 1)
        boardToEdit[yOff][xOff] = player->piece->pieceArray[i][j];
    }
  }  
}

// Draws all the blocks on the tetris board on the led display
void Game::updateBoard() {     
    byte boardWithPiece[ROWS][COLS];
    copyBoard(boardWithPiece);
    placePiece(boardWithPiece);

    byte blocksToDisplay[ROWS];
    serialize(boardWithPiece, blocksToDisplay);

    // Redraws any rows on the board that have changed since last frame
    for (int i = 0; i < ROWS; i++) {
        int row = (i % 8);
        int address = 3 - (i / 8);

        if (isFirstFrame || prevDisplayedBlocks[i] != blocksToDisplay[i])
            ledMatrix->setColumn(address, row, blocksToDisplay[i]);
    }

    // copies the blocks from this frame into prevDisplayedBlocks
    for (int i = 0; i < ROWS; i++) {
        prevDisplayedBlocks[i] = blocksToDisplay[i];
    }

    isFirstFrame = 0;
}

void Game::deleteLine(int lineToDelete) {
  for (int j = lineToDelete; j > 0; j--) {
    for (int i = 0; i < COLS; i++) {
      board[j][i] = board[j - 1][i];
    }
  }
}

void Game::deleteClearedLines() {
  for(int j = ROWS - 1; j > 0; j--) {
    int i = 0;
    while (i < COLS) {
      if (board[j][i] == 0){
        break;
      }
      i++;
    }  
    if (i == COLS) {
      deleteLine(j);
      linesCleared++;
    }
  } // end of for loop


  // Update the player's score:
  if(linesCleared == 4) {
    player->score += 2000;
  } else {
    player->score += (100 * linesCleared);
  }
  String toSend = "!updateScore ";
  commandToSend = toSend + player->score + "\n";
}


void Game::recieveGarbage(int lines) {
  int randomNum = random(0,8);
  // i have no idea if this works or not, should move everything up by one
  for (int j = ROWS - 1; j > 0; j--) {
    for (int i = 0; i < COLS; i++) {
      board[j][i] = board[j-1][i];
    }
  }
  //this works for sure though
  for (int j = ROWS - 1; j > ROWS - lines - 1; j--) { // set 1 block to be empty in garbage
    for(int i = 0; i < COLS; i++){
      board[j][i] = 1;
    }
    board[j][randomNum] = 0;
  }
}

void Game::sendGarbage(){ 
  if(linesCleared == 1 || linesCleared == 0) {
     commandToSend = "";
     linesCleared = 0;
  } else if (linesCleared == 2) {
    commandToSend = "!sendGarb 1\n";
    linesCleared = 0;
  } else if (linesCleared == 3) {
    commandToSend = "!sendGarb 2\n";
    linesCleared = 0;
  } else if (linesCleared == 4) {
    commandToSend = "!sendGarb 4\n";
    linesCleared = 0;
  }
}
