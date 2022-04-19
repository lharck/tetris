#include "Game.h"
Game* game;

void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(A0));
    random(0, 7);
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
