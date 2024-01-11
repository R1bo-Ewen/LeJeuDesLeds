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


const int BUTTONS[numLeds] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6};
const int LEDS[numLeds] = {LED1, LED2, LED3, LED4, LED5, LED6};

bool ledState[numLeds];
bool buttonState[numLeds];
bool lastButtonState[numLeds];
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
    lastButtonState[i] = false;
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
    ledState[i] = random(0, 2);
  }
}

bool hasButtonStateChanged(bool currentState, int i) {
  if (currentState != lastButtonState[i]) {
    return true;
  }
  else {
    return false;
  }
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
    digitalWrite(LEDS[i], !ledState[i]);
  }
}

bool checkLedsState(){
  int totalLedsOn = 0;

  for (int i = 0; i < numLeds; i++) {
    totalLedsOn += ledState[i];
  }

  if (totalLedsOn == numLeds) {
    return true;
  }
  else {
    return false;
  }
}

void LedsGame(){
  for (int i = 0; i < numLeds; i++) {
    bool isButtonPressed = !digitalRead(BUTTONS[i]);
    
    if (hasButtonStateChanged(isButtonPressed, i) && isDebounceDelayOver(i)) {
      lastButtonState[i] = isButtonPressed;
      lastDebounceTime[i] = millis();

      if (isButtonPressed) {
        if(i == 0) {
          ledState[0] = !ledState[0];
          ledState[1] = !ledState[1];
        }
        else if (i == numLeds-1) {
          ledState[numLeds-2] = !ledState[numLeds-2];
          ledState[numLeds-1] = !ledState[numLeds-1];
        }
        else {
          ledState[i-1] = !ledState[i-1];
          ledState[i] = !ledState[i];
          ledState[i+1] = !ledState[i+1];
        }
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
  for (int i = 0; i < 7; i++) {
    for (int i = 0; i < numLeds; i++) {
      ledState[i] = !ledState[i];
    }
    refreshLeds();
    pause(150);
  }
}

void startsBlink(){
  resetLedsState();
  for (int i = 0; i < 3; i++) {
    for (int i = 0; i < numLeds; i++) {
      ledState[i] = !ledState[i];
      refreshLeds();
      pause(100);
    }
  }
  startTime = millis();
}

void endsBlink(){
  resetLedsState();
  for (int i = 0; i < 6; i++) {
    for (int i = 0; i < numLeds; i++) {
      ledState[i] = !ledState[i];
    }
    refreshLeds();
    pause(750);
  }
}

void displayScore(){
  resetLedsState();
  int currentScoreBit = 32;
  int reste;
  for (int i = 0; i < numLeds; i++) {
    reste = score - currentScoreBit;
    if (reste>=0){
      ledState[i] = true;
    }
    else{
      reste += currentScoreBit;
    }
    currentScoreBit = currentScoreBit/2;
  }
  refreshLeds();
}

void resetLedsState(){
  for (int i = 0; i < 3; i++) {
    for (int i = 0; i < numLeds; i++) {
      ledState[i] = false;
    }
  }
  refreshLeds();
}

void pause(float time){
  float pauseTime = millis();
  while (millis()-pauseTime < time){}
}
