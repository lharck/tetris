#include "Game.h"

#include "Arduino.h"

void printBinary(byte inByte) {
    for (int b = 7; b >= 0; b--) {
        //Serial.print(bitRead(inByte, b));
    }
    //Serial.println();
}

Game::Game() {
    state = States::WaitingForStart;
    ledMatrix = new LedControl(DIN, CLK, CS, 4);

    for (int i = 0; i < 4; i++) {
        // turn off power saving, enables display
        ledMatrix->shutdown(i, false);

        // sets brightness (0~15 possible values)
        ledMatrix->setIntensity(i, 1);

        // clear screen
        ledMatrix->clearDisplay(i);
    }

    initBoard();
    player = new Player(&board);
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
  player->getNewPiece();
  updateBoard();
}

// Copies board and piece into into result array:
void Game::cloneBoardWithPiece(byte (&boardWithPiece)[ROWS][COLS]) {
    // Copies board into boardWithPiece array
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            boardWithPiece[i][j] = board[i][j];
        }
    }

    // Copies piece onto boardWithPiece array
    // Starting at it's current position
    for (int i = 0; i < player->piece->height; i++) {
        for (int j = 0; j < player->piece->width; j++) {
            int xOff = player->piece->x + j;
            int yOff = player->piece->y + i;
            boardWithPiece[yOff][xOff] = player->piece->pieceArray[i][j];
        }
    }
}

void Game::placePiece(){
  
}

// Draws all the blocks on the tetris board on the led display
void Game::updateBoard() { 
    state = States::UpdatingBoard;

    byte boardWithPiece[ROWS][COLS];
    cloneBoardWithPiece(boardWithPiece);

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
    state = States::Idle;
}
