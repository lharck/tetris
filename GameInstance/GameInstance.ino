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
  if(command == "!stop \n"){
    game->reset();
    game->start();
  }
  else if(command == "!start \n"){
    game->reset();
  }
}


void loop() {
  if(game->state != game->States::GAME_IN_PROGRESS){return;}
  
  String actionPerformed = game->player->checkInputChange();
  bool timeToDropPiece = (bool)(millis() - lastSoftDropTime > SOFT_DROP_INTERVAL);

  if(actionPerformed != "None"){   
    game->updateBoard();
    lastSoftDropTime = millis() - (SOFT_DROP_INTERVAL/2);
  }

  else{
     // if it's time to drop the piece:
    if (timeToDropPiece) {
      game->player->piece->move(0,1);
      lastSoftDropTime = millis();
      game->updateBoard();
    }
  }

  // if we've fallen to the bottom of the board or on top of a piece:
  if(game->player->piece->hasFallen()){
    if(game->player->piece->yBoardUp == 0){
      String toSend = "!died ";
      toSend += game->player->score + "\n";
      game->commandToSend = toSend;
      game->reset();
      return;
    }
    game->placePiece(game->board);
    game->deleteClearedLines();  
    game->player->getNewPiece();
    game->updateBoard(); 
  }    
}
