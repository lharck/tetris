#include "Game.h"
#include "RandomSeed.h"
#include <avr/eeprom.h>
#include "Wire.h"

Game* game;
uint32_t reseedRandomSeed EEMEM = 0xFFFFFFFF;

void setup() {
    Serial.begin(9600);
    RandomSeed.reseedRandom(&reseedRandomSeed);
    
    game = new Game();

    Serial.begin(9600);  

    // REMEMBER TO CHANGE THIS TO YOUR SPECIFIED ID:
    // LUCY: 1, MATT: 2, JAMES: 3
    Wire.begin(1);
    Wire.onReceive(onDataReceived);
    Wire.onRequest(onDataRequested);
  
    game->start();   
}


unsigned long lastSoftDropTime = millis(); 
const int SOFT_DROP_INTERVAL = 1000;

// Function which is called when Shijil's arduino requests data
void onDataRequested() {
  if(game->commandToSend != ""){
    Wire.write(game->commandToSend.c_str());
    game->commandToSend = "";
  }
}


// Function which is called when we receive 
// data from shijil's arduino
void onDataReceived(int numBytesRequested){  
  String command = "";
  
  while(Wire.available()){
  command += (char)Wire.read();
  }
  Serial.println(command);
}


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
