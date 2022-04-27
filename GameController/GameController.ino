#include <Wire.h>  

String readString;                                                          
byte dataIn;     
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
int gameButton = 8;

void setup()
{
  Wire.begin();                                                             
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(gameButton, INPUT);
  lcd.setCursor(0, 1);
  lcd.print("Button Pressed");
  /*
  Serial.println("Type On to turn on the LED and Off to shut it down!");
  Serial.println("Welcome to our Multiplayer Tetris Game!");   
  Serial.println("Press the start button to begin the game.");  
  Serial.println("Press the stop button to stop the game.");  
  */
}

int NUM_PLAYERS = 3;
int playerScore = 0;
int numPlayersAlive = 0; // variable to represent how many players are alive in the game
bool gameInProgress = false; // variable which tells us if the game is already in progress
int highScore = 0;
int playerIdHighScore = 0;
String cmdArray[20];
int stringLength = 0, itr = 0;


// Split the string into substrings
void split(String str){
  int len = 0;
  for(int i = 0; i < 20; i++){
    cmdArray[i] = "";
  }

  while (str.length() > 0){
    // look to see if we find a space in the string
    int index = str.indexOf(' ');

    // if we don't find a space 
    if (index == -1){
      cmdArray[len++] = str;
      break;
    }
    else{
      cmdArray[len++] = str.substring(0, index);
      str = str.substring(index+1);
    }
  }
}

// Reads a command from the wire
// Commands must be a string starting with '!' 
// and end with '\n'
void processCommand(int playerId){
  Serial.print((String) "Command from player #" + playerId + ": ");
  char c;
  String command = "";
  while (Wire.available()) {
    c = Wire.read();
    Serial.print(c);
    command = command + c;

    if(c=='\n')
      return;
  }

  split(command);
  
  if (cmdArray[0] == "!died") {
    int numPlayersAlive = numPlayersAlive - 1;
    String tempNum = "";
    tempNum = cmdArray[1];
    playerScore = tempNum.toInt();
    Serial.print((String) "Player " + playerId + "has died. \n");
//    sendToPlayer(playerId, "What is your Final Score?");  
//    Wire.requestFrom(playerId, 32);
//    char x;
//    while (Wire.available()) {
//      x = Wire.read();
//    }
//    playerScore = x;
  }

  if (cmdArray[0] == "!updateScore") {
    String tempNum2 = "";
    tempNum2 = cmdArray[1];
    playerScore = tempNum2.toInt();
    Serial.print((String) "Player " + playerId + "has a new score of " + playerScore + "\n");
  }

  if (numPlayersAlive == 0) {
    for(int i = 1; i < NUM_PLAYERS+1; i++){
      sendToPlayer(i, "What is your Score?"); 
      Wire.requestFrom(i, 32);
      char y;
      while (Wire.available()) {
        y = Wire.read();
      }
      playerScore = y;
      if (playerScore > highScore) {
        highScore = playerScore;
        playerIdHighScore = i;
      }
    }
  Serial.print((String) "Player " + playerIdHighScore + "has the highest score of " + highScore + "\n");
  gameInProgress = false;
  numPlayersAlive = 0;
  }
  String garbageLines = "";
  if (cmdArray[0] == "!sendGarb") {  
    garbageLines = cmdArray[1];
    if (playerId == 1){
      sendToPlayer(2, (String) "!recieveGarb " + garbageLines); 
    } else if (playerId == 2) {
        sendToPlayer(3, (String) "!recieveGarb " + garbageLines); 
    } else {
        sendToPlayer(1, (String) "!recieveGarb " + garbageLines); 
    }
  }
}




void sendToPlayer(int playerId, String dataToSend){
  Wire.beginTransmission(playerId);
  Wire.write(dataToSend.c_str());
  Wire.endTransmission(); 
}


// requests 'numBytes' bytes from each player
// player can send less than 'numBytes' bytes
// just the rest of the bytes will be garbage
void requestDataFromPlayers(int numBytes){
  for(int i = 1; i < NUM_PLAYERS+1; i++){
    Wire.requestFrom(i, numBytes);
    char firstChar = Wire.read();
    
    // if the character that we read in is '!'
    // that means the player sent a command
    // so process the command 
    if(firstChar == '!'){
      processCommand(i);  
    }
    
    //Process any extra bytes read in
    while (Wire.available()) {
      char c = Wire.read(); 
    }
    
  }  
}

//
//void gameButtonPressed(){
//  if (gameInProgress) {
//    for(int i = 1; i < NUM_PLAYERS+1; i++){
//      sendToPlayer(i, "!start \n");  
//    }
//    numPlayersAlive = 3;
//  } else {
//    for(int i = 1; i < NUM_PLAYERS+1; i++){
//      sendToPlayer(i, "!stop \n");  
//    }
//    numPlayersAlive = 0;
//  }
//  gameInProgress = !gameInProgress;
//}

void gameButtonPressed(){
   // change game in progress to be the opposite of what it currently is
  gameInProgress = !gameInProgress;

  // if the game isn't in progress
  if (!gameInProgress) {
    Serial.println("Game Started \n");
    // start the game for all players
    for(int i = 1; i < NUM_PLAYERS+1; i++){
      sendToPlayer(i, "!start \n");
    }
    // set num players alive to be 3
    numPlayersAlive = 3;

  // otherwise, if the game is currently in progress
  } else {
    Serial.println("Game Stopped \n");
    // stop the game for each players
    for(int i = 1; i < NUM_PLAYERS+1; i++){
      sendToPlayer(i, "!stop \n");
    }

    // since the game stopped nobody is alive anymore vv
    numPlayersAlive = 0;
  }
}

void scroll(int line, String quote) {
  stringLength = quote.length();
  
  for (itr = 16; itr >= 0; itr--) {
    lcd.setCursor(0, line - 1);
    lcd.print(quote);
    lcd.setCursor(itr, line);
    lcd.print(quote.substring(0, 16));
    delay(320);
  }
  
  itr = 0;
  
  while (itr != stringLength) {
    itr++;
    lcd.setCursor(0, line);
    lcd.print(quote.substring(itr,itr+16));
    delay(320);
  }
}



unsigned int timeSinceLastSent = millis(); 
//int currentButtonState = digitalRead(gameButton);
bool previousButtonState = LOW;


void loop(){
//  int buttonState = digitalRead(gameButton);
//  bool previousButtonState = LOW:
//  if (buttonState != previous) {
//    gameButtonPressed();
//  } else {
//       for(int i = 1; i < NUM_PLAYERS+1; i++){
//          sendToPlayer(i, "Game Stopped");  
//    }
//  }
    //lcd.setCursor(0, 0);
    
    int currentButtonState = digitalRead(gameButton);
    //scroll(1, "Button Pressed");
    if (currentButtonState != previousButtonState) {
      Serial.println("We started or stopped pressing a button");
      //lcd.print("Button Pressed");
      delay(100);
      // if we started pressing button
      if(currentButtonState == HIGH){
          // Serial.println("Game Started \n");
          gameButtonPressed();
      } 
//      else if(currentButtonState == LOW) {
//          Serial.println("Game Stopped \n");
//          gameInProgress = !gameInProgress;
//          gameButtonPressed();
//      }
    }
    
    
    


  // Sends 'hi from shijil's arduino' to each player
//  for(int i = 1; i < NUM_PLAYERS+1; i++){
//    sendToPlayer(i, "Hi from Shijil's Arduino.\n");  
//  }
  
  // Requests 32 bytes from each player
  // Processes a command if the data we got from the player is a command
  requestDataFromPlayers(32); 
}
