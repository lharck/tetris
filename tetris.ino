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

void loop() {
    if (game->state == game->States::GameOver) {
    } else if (game->state == game->States::WaitingForInput) {
        bool inputPressed = game->player->checkInputChange();

        if (inputPressed) {
            game->updateBoard();
        }
    }
}
