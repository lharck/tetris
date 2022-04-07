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
  // piece->hardDrop();
}

void Player::onLeftRotatePressed(){
  piece->rotate("Left");
}

void Player::onRightRotatePressed(){
  piece->rotate("Right");
} 

void Player::onJoystickPressed(){
  // piece->hardDrop();
} 

bool Player::onJoystickMoved(int x, int y){
  switch(x){
    case 0: piece->move(1,0); return true;
    case 6: piece->move(-1,0); return true;
    default: break;
  } 

  if(y <= 1){
    piece->move(0,1);
    return true;
  }

  return false;
}


bool Player::processJoystick(){  
  int xValue = analogRead(JOY_X_PIN);
  int yValue = analogRead(JOY_Y_PIN);
  int xMap = map(xValue, 0, 1023, 0, 7);
  int yMap = map(yValue, 0, 1023, 7, 0); 

  return onJoystickMoved(xMap, yMap);
}

bool Player::processButtons(){
  for(int i = 0; i < NUM_BUTTONS; i++){
    previousButtonStates[i] = currentButtonStates[i];
    currentButtonStates[i] = digitalRead(buttonPins[i]);
    
    if(previousButtonStates[i] != currentButtonStates[i] && currentButtonStates[i] == 0){
      // ignores the button press if we pressed a button less than 10 ms about 
      // to prevent 'contact bouncing'
      if(millis() - lastButtonPress < 10){return false;} 
      lastButtonPress = millis();
      
      switch(buttonPins[i]){
          case HARD_DROP_PIN: onDropButtonPressed(); break;
          case LEFT_ROTATE_PIN: onLeftRotatePressed(); break; 
          case RIGHT_ROTATE_PIN: onRightRotatePressed(); break;
          case JOY_CLICK_PIN: onJoystickPressed(); break;
      }

      return true;
    }
  }

  return false;
}

bool Player::checkInputChange(){
  return processJoystick() || processButtons();
}
