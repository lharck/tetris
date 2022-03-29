#include "Arduino.h" 
#include "Player.h"

Player::Player(){
    piece = nullptr;
    // this->board = board;
    // score = 0;
}

void Player::getNewPiece(const int NUM_COLS){
    int chosenPieceType = random(0,7);
    piece = new Piece(chosenPieceType, NUM_COLS);
}

// void Player::placePiece(Piece* piece){
    
// }

// void Player::onEliminiated(){

// }

// void Player::onLeftRotatePressed(){

// }

// void Player::onRightRotatePressed(){

// } 

// void Player::onJoystickMoved(int x, int y){

// }

// void Player::checkInputChanged(){

// }
