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
const int SOFT_DROP_INTERVAL = 1000;
void loop() {

    if (game->state == game->States::GameOver) {
      
    } 
    else if (game->state == game->States::Idle) {
        String actionPerformed = game->player->checkInputChange();
        bool timeToDropPiece = (bool)(millis() - lastSoftDropTime > SOFT_DROP_INTERVAL);

        if(actionPerformed == "HardDrop"){
          game->placePiece(game->board); // put the piece onto the board  
          game->deleteClearedLines();
          game->player->getNewPiece(); // give the player a new piece
          game->updateBoard();
        }
        else if (actionPerformed != "None") {
            game->updateBoard();
            lastSoftDropTime = millis() - (SOFT_DROP_INTERVAL/2);
        }

        if (timeToDropPiece) {
           game->player->piece->move(0,1);
           game->deleteClearedLines();
           lastSoftDropTime = millis();                 
           game->updateBoard();
        }

        if(game->player->piece->hasFallen()){
            game->placePiece(game->board); // put the piece onto the board
            game->deleteClearedLines();  
            game->player->getNewPiece(); // give the player a new piece
        }
    }
}
