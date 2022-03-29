#include "Game.h"
Game* game; 

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
  game = new Game();
  game->start();
}

void loop(){
  // print a random number from 10 to 19


}