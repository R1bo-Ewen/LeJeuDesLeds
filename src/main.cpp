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

unsigned long lastDebounceTime[numLeds];
const unsigned long debounceDelay = 50;

// put function declarations here:
void randomizeLedStates();
bool hasButtonStateChanged(bool currentState, int i);
bool isDebounceDelayOver(int i);
void refreshLeds();
bool checkLedsState();


void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(BUTTONS[i], INPUT_PULLUP);
    pinMode(LEDS[i], OUTPUT);
    lastDebounceTime[i] = 0;
  }

  randomizeLedStates();
}


void loop() {
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
  int totalLedsOn = 0;

  for (int i = 0; i < numLeds; i++) {
    // totalLedsOn += ledState[i];
  }

  if (totalLedsOn == numLeds) {
    return true;
  }
  else {
    return false;
  }
}