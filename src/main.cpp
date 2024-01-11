#include <Arduino.h>

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
void randomizeLedStates();
bool hasButtonStateChanged(bool currentState, int i);
bool isDebounceDelayOver(int i);
void refreshLeds();
bool checkLedsState();
void LedsGame();
void startsBlink();
void startNewLevel();
void levelsBlink();
void endsBlink();
void endGame();
void displayScore();
void resetLedsState();
void pause(float time);


void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(BUTTONS[i], INPUT_PULLUP);
    pinMode(LEDS[i], OUTPUT);
    lastDebounceTime[i] = 0;
  }
  Serial.begin(9600);
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


void randomizeLedStates() {
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
  bool lastState = lastButtonState & FLAGS[i];
  return !(currentState == lastState);
}

void startNewLevel(){
  gameOn = false;
  score++;
  Serial.print(score);
  levelsBlink();
  randomizeLedStates();
  gameOn = true;
}

bool isDebounceDelayOver(int i) {
  if (millis() - lastDebounceTime[i] > debounceDelay) {
    return true;
  }
  else {
    return false;
  }
}

void refreshLeds(){
  for (int i = 0; i < numLeds; i++) {
    bool state = ledState & FLAGS[i];
    digitalWrite(LEDS[i], !state);
  }
}

bool checkLedsState(){
  bool hasWon = (ledState == 0b00111111);
  return hasWon;
}

void LedsGame(){
  for (int i = 0; i < numLeds; i++) {
    bool isButtonPressed = !digitalRead(BUTTONS[i]);
    
    if (hasButtonStateChanged(isButtonPressed, i) && isDebounceDelayOver(i)) {
      lastDebounceTime[i] = millis();

      if (isButtonPressed) {
        // Write 1
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
    startNewLevel();
  }
  endTime = millis();
  timePassed = endTime - startTime;
  if (timePassed>=30000){
    endGame();
  }
}

void endGame(){
  gameOn = false;
  endsBlink();
  displayScore();
  score = 0;
  startOn = true;
}

void levelsBlink(){
  resetLedsState();
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < numLeds; j++) {
      ledState ^= FLAGS[j];
    }
    refreshLeds();
    pause(150);
  }
}

void startsBlink(){
  resetLedsState();
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < numLeds; j++) {
      ledState ^= FLAGS[j];
      refreshLeds();
      pause(100);
    }
  }
  startTime = millis();
}

void endsBlink(){
  // Probleme de blink 
  // resetLedsState();
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < numLeds; j++) {
      ledState ^= FLAGS[j];
    }
    refreshLeds();
    pause(750);
  }
}

void displayScore(){
  // score a inverser
  resetLedsState();
  ledState = score;
  refreshLeds();
}

void resetLedsState(){
  for (int j = 0; j < numLeds; j++) {
    ledState ^= FLAGS[j];
  }
  refreshLeds();
}

void pause(float time){
  float pauseTime = millis();
  while (millis()-pauseTime < time){}
}
