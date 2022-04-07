#include "Arduino.h" 
#include "Player.h"

Player::Player(){
    for(byte pin : buttonPins){
      pinMode(pin, INPUT_PULLUP);
    }

    piece = nullptr;
    score = 0;
}

void Player::getNewPiece(const int NUM_COLS){
  int chosenPieceType = random(0,7);
  piece = new Piece(chosenPieceType, NUM_COLS);
}

// void Player::onEliminiated(){

// }

void Player::onDropButtonPressed(){

}

void Player::onLeftRotatePressed(){
}

void Player::onRightRotatePressed(){
  
} 

void Player::onJoystickPressed(){
  
} 

void Player::onJoystickMoved(int x, int y){
  switch(x){
    case 0: piece->move(-1,0); return;
    case 6: piece->move(1,0); return;
    default: break;
  } 

  if(y==0){
    piece->move(0,1);
  }
}


void Player::processJoystick(){  
  int xValue = analogRead(JOY_X_PIN);
  int yValue = analogRead(JOY_Y_PIN);
  int xMap = map(xValue, 0, 1023, 0, 7);
  int yMap = map(yValue, 0, 1023, 7, 0); 

  onJoystickMoved(xMap, yMap);
}

void Player::processButtons(){
  for(int i = 0; i < NUM_BUTTONS; i++){
    previousButtonStates[i] = currentButtonStates[i];
    currentButtonStates[i] = digitalRead(buttonPins[i]);
    
    if(previousButtonStates[i] != currentButtonStates[i] && currentButtonStates[i] == 0){
        // ignores the button press if we pressed a button less than 10 ms about 
        // to prevent 'contact bouncing'
        if(millis() - lastButtonPress < 10){return;} 
        lastButtonPress = millis();
      
      switch(buttonPins[i]){
          case HARD_DROP_PIN: onDropButtonPressed(); break;
          case LEFT_ROTATE_PIN: onLeftRotatePressed(); break; 
          case RIGHT_ROTATE_PIN: onRightRotatePressed(); break;
          case JOY_CLICK_PIN: onJoystickPressed(); break;
      }
    }
  }
}

void Player::checkInputChange(){
  processJoystick();
  processButtons();
}
