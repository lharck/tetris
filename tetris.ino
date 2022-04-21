#include "Game.h"
#include "RandomSeed.h"
#include <avr/eeprom.h>

Game* game;
uint32_t reseedRandomSeed EEMEM = 0xFFFFFFFF;

void setup() {
    Serial.begin(9600);
    RandomSeed.reseedRandom(&reseedRandomSeed);
    
    game = new Game();
    game->start();
}


unsigned long lastSoftDropTime = millis(); 

void loop() {
    
    if (game->state == game->States::GameOver) {
      
    } 
    else if (game->state == game->States::Idle) {
        bool inputPressed = game->player->checkInputChange();
        bool timeToDropPiece = (bool)(millis() - lastSoftDropTime > 1000);
        
        if (inputPressed) {
            game->updateBoard();
            lastSoftDropTime = millis();
        }
        else if (timeToDropPiece) {
            if(game->player->piece->hasBlocksBelow()){
              game->placePiece(); // put the place onto the board
              game->player->getNewPiece(); // give the player a new piece
            }
            else{
              // perform soft drop:
              game->player->piece->move(0,1);
              lastSoftDropTime = millis();             
            }
             
           game->updateBoard();
        }
    }
}
