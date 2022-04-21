#include "Player.h"

#include "Arduino.h"

Player::Player(byte (*pBoard)[32][8]) {
    this->pBoard = pBoard;

    lastButtonPressTime = millis();
    lastSoftDropTime = millis();
    prevJoystickDirection = "";
    
    for (byte pin : buttonPins) {
        pinMode(pin, INPUT_PULLUP);
    }

    piece = nullptr;
    score = 0;
}

void Player::getNewPiece() {
    int chosenPieceType = random(0, 7);
    piece = new Piece(chosenPieceType, pBoard);
}

void Player::onDropButtonPressed() { piece->hardDrop(); }

void Player::onLeftRotatePressed() { piece->rotate("Left"); }

void Player::onRightRotatePressed() { piece->rotate("Right"); }

void Player::onJoystickPressed() { piece->hardDrop(); }

String Player::onJoystickMoved(int x, int y) { 
    if (x >= 5) {
        if(prevJoystickDirection == "XRIGHT"){
            return "None";
        }
        piece->move(1, 0);
        prevJoystickDirection = "XRIGHT";
        return "MovedJoystick";
    } else if (x <= 1) {
        if(prevJoystickDirection == "XLEFT"){
          return "None";
        }
        piece->move(-1, 0);
        prevJoystickDirection = "XLEFT";
        return "MovedJoystick";
    }

    if (y <= 2) {
        if(prevJoystickDirection == "YDOWN" && millis() - lastSoftDropTime < 50){
          return "None";
        }
        prevJoystickDirection = "YDOWN";
        piece->move(0, 1);
        lastSoftDropTime = millis();
        return "MovedJoystick";
    }
    
    prevJoystickDirection = "";
    return "None";
}

String Player::processJoystick() {
    int xValue = analogRead(JOY_X_PIN);
    int yValue = analogRead(JOY_Y_PIN);
    int xMap = map(xValue, 0, 1023, 0, 7);
    int yMap = map(yValue, 0, 1023, 7, 0);

    return onJoystickMoved(xMap, yMap);
}

String Player::processButtons() {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        previousButtonStates[i] = currentButtonStates[i];
        currentButtonStates[i] = digitalRead(buttonPins[i]);

        if (previousButtonStates[i] != currentButtonStates[i] &&
            currentButtonStates[i] == 0) {
            // ignores the button press if we pressed a button less than 10 ms
            // to prevent 'contact bouncing'
            if (millis() - lastButtonPressTime < 10) {
                return "None";
            }
            
            lastButtonPressTime = millis();

            switch (buttonPins[i]) {
                case HARD_DROP_PIN:
                    onDropButtonPressed();
                    return "HardDrop";
                case LEFT_ROTATE_PIN:
                    onLeftRotatePressed();
                    return "Rotated";
                case RIGHT_ROTATE_PIN:
                    onRightRotatePressed();
                    return "Rotated";
                case JOY_CLICK_PIN:
                    onJoystickPressed();
                    return "HardDrop";
            }
        }
    }

    return "None";
}

String Player::checkInputChange() {
    String actionPerformed = processJoystick();
    
    if(actionPerformed == "None")
      actionPerformed = processButtons();
      
    return actionPerformed;
}
