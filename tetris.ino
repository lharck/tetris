#include "Game.h"
Game* game; 

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  random(0,7); 
  Serial.println(random(0,7));
  game = new Game();
  game->start();
}

void loop(){
  bool inputPressed = game->player->checkInputChange();

  if(inputPressed){
   // Serial.println("UPDATING");
    game->updateBoard();
  }
}
