#include <Arduino.h>
#include <unistd.h>

// Déclaration des variables des boutons et des leds ainsi que leur nombre
#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4
#define BUTTON4 5
#define BUTTON5 6
#define BUTTON6 7
#define LED1 8
#define LED2 9
#define LED3 10
#define LED4 11
#define LED5 12
#define LED6 13
#define numLeds 6

#define FLAG1 0b00000001
#define FLAG2 0b00000010
#define FLAG3 0b00000100
#define FLAG4 0b00001000
#define FLAG5 0b00010000
#define FLAG6 0b00100000

#define FLAG1 0b00000001
#define FLAG2 0b00000010
#define FLAG3 0b00000100
#define FLAG4 0b00001000
#define FLAG5 0b00010000
#define FLAG6 0b00100000

const int BUTTONS[numLeds] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6};
const int LEDS[numLeds] = {LED1, LED2, LED3, LED4, LED5, LED6};

char ledState = 0;
char buttonState = 0;
char lastButtonState = 0;
const char FLAGS[numLeds] = {FLAG1, FLAG2, FLAG3, FLAG4, FLAG5, FLAG6};

bool gameOn = false;
bool startOn = true;
float startTime;
float endTime;
float timePassed;
int score;

unsigned long lastDebounceTime[numLeds];
const unsigned long debounceDelay = 50;

// put function declarations here:

// Function for the game to work
void LedsGame();
void randomizeLedStates();
bool hasButtonStateChanged(bool currentState, int i);
bool isDebounceDelayOver(int i);
bool checkLedsState();
void startNewLevel();
void endGame();

// Function for the visual feedback
void refreshLeds();
void resetLedsState();
void startsBlink();
void levelsBlink();
void endsBlink();
void displayScore();

// Utility function
int swapBits(int myByte);


void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(BUTTONS[i], INPUT_PULLUP);
    pinMode(LEDS[i], OUTPUT);
    lastDebounceTime[i] = 0;
  }
}


void loop() {
  if (gameOn){
    LedsGame();
  }
  else{
    if (digitalRead(BUTTONS[0]) != 1 & startOn){
      startsBlink();
      randomizeLedStates();
      gameOn = true;
      startOn = false;
    }
  } 
}

//---------------------Function for the Game----------------------------------//

void LedsGame(){
  // The main fuction the game
  for (int i = 0; i < numLeds; i++) {
    bool isButtonPressed = !digitalRead(BUTTONS[i]);
    
    if (hasButtonStateChanged(isButtonPressed, i) && isDebounceDelayOver(i)) {
      // We check every button and if one is pressed we change the state of 
      // it's light and to the ones next to it
      lastDebounceTime[i] = millis();

      if (isButtonPressed) {
        lastButtonState |= FLAGS[i];

        if(i == 0) {
          ledState ^= FLAGS[i];
          ledState ^= FLAGS[i+1];
        }
        else if (i == numLeds-1) {
          ledState ^= FLAGS[numLeds-2];
          ledState ^= FLAGS[numLeds-1];
        }
        else {
          ledState ^= FLAGS[i-1];
          ledState ^= FLAGS[i];
          ledState ^= FLAGS[i+1];
        }
      }
      else {
        // Write 0
        lastButtonState ^= FLAGS[i];
      }
    }

    refreshLeds();
  }
  if (checkLedsState()){
    // Checking if the level is finished and if so, visualy informing the player
    // And starting a new level 
    startNewLevel();
  }
  // Checking if the time is out and ending the game if it's the case
  endTime = millis();
  timePassed = endTime - startTime;
  if (timePassed>=30000){
    endGame();
  }
}

void randomizeLedStates() {
  // Function called at the beginning of every level,
  // It attribute a random state to every light
  for (int i = 0; i < numLeds; i++) {
    randomSeed(analogRead(0));
    char newState = random(0, 2);
    char newStateFlag = newState << i;

    if (newState) {
      // If new state is ON, write ledState with OR
      ledState |= newStateFlag;
    }
  }
}

bool hasButtonStateChanged(bool currentState, int i) {
  // Function checking if the button is pressed or not and returning
  // True of False
  bool lastState = lastButtonState & FLAGS[i];
  return !(currentState == lastState);
}

bool isDebounceDelayOver(int i) {
  // Function used to check if an infromation respect a certain time between
  // two change of state. Used so the button don't get pressed twice
  if (millis() - lastDebounceTime[i] > debounceDelay) {
    return true;
  }
  else {
    return false;
  }
}

bool checkLedsState(){
  // Function checking if the all leds are activated and of so return true
  bool hasWon = (ledState == 0b00111111);
  return hasWon;
}

void startNewLevel(){
  // Function called when a player light all light,
  // We're augmenting his score and restaring a new level
  gameOn = false;
  score++;
  Serial.print(score);
  levelsBlink();
  randomizeLedStates();
  gameOn = true;
}

void endGame(){
  // Function ending the current game and letting the player start a new one
  gameOn = false;
  endsBlink();
  displayScore();
  score = 0;
  startOn = true;
}

// -----------------Function for the visual feedback--------------------------//

void refreshLeds(){
  // Fonction refreshing the visual feedback of the leds to their current value in the list 
  for (int i = 0; i < numLeds; i++) {
    bool state = ledState & FLAGS[i];
    digitalWrite(LEDS[i], !state);
  }
}

void resetLedsState(){
  // Function called at the begging of all visual feedback,
  // turning off all light
  ledState = 0;
  refreshLeds();
}

void startsBlink(){
  // Function called in need of a visual feed back, used when a the player start a game
  resetLedsState();
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < numLeds; j++) {
      ledState ^= FLAGS[j];
      refreshLeds();
      delay(100);
    }
  }
  startTime = millis();
}

void levelsBlink(){
  // Function called in need of a visual feed back, used when a level is complet
  resetLedsState();
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < numLeds; j++) {
      ledState ^= FLAGS[j];
    }
    refreshLeds();
    delay(150);
  }
}

void endsBlink(){
  // Function called in need of a visual feed back, used when a the time is out
  // and the game stoped
  resetLedsState();
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < numLeds; j++) {
      ledState ^= FLAGS[j];
    }
    refreshLeds();
    delay(750);
  }
}

void displayScore(){
  // Function called in need of a visual feed back, used to show the score
  // score a inverser
  resetLedsState();
  ledState = swapBits(score);
  refreshLeds();
}

int swapBits(int myByte){
  // Swap the first 4 bits with the last 4 bits
  myByte = ((myByte & 0b11110000) >> 4) | ((myByte & 0b00001111) << 4); //Now a = 0xEFGHABCD

  // Next swap pairs of 2 bits
  myByte = ((myByte & 0b11001100) >> 2) | ((myByte & 0b00110011) << 2); //Now a = 0xGHEFCDAB

  // Finally, swap every other bit. 
  myByte = ((myByte & 0b10101010) >> 1) | ((myByte & 0b01010101) << 1); //Now a = 0xHGFEDCBA

  // Shifting to avoid missing leds
  myByte = myByte >> (8 - int(numLeds));

  return myByte;
}