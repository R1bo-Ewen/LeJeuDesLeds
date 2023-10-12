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

bool ledStates[numLeds];
bool buttonStates[numLeds];
bool lastButtonStates[numLeds];

unsigned long lastDebounceTimes[numLeds];
const unsigned long debounceDelay = 50;


// put function declarations here:
void randomizeStates();


void setup() {

  for (int i = 0; i < numLeds; i++)
  {
    pinMode(BUTTONS[i], INPUT_PULLUP);
    pinMode(LEDS[i], OUTPUT);
    lastButtonStates[i] = false;
    lastDebounceTimes[i] = 0;
  }

  randomizeStates();

}


void loop() {

  for (int i = 0; i < numLeds; i++)
  {
    bool currentButtonPressed = digitalRead(BUTTONS[i]);

    if (currentButtonPressed != lastButtonStates[i]) {
      lastDebounceTimes[i] = millis();
    }

    if (millis() - lastDebounceTimes[i] > debounceDelay) {
      if (currentButtonPressed != buttonStates[i]) {
        buttonStates[i] = currentButtonPressed;
        if (currentButtonPressed == true) {
          ledStates[i] = !ledStates[i];
        }
      }
    }

    lastButtonStates[i] = currentButtonPressed;

    digitalWrite(LEDS[i], !ledStates[i]);
  }
}


void randomizeStates() {

  for (int i = 0; i < numLeds; i++)
  {
    randomSeed(analogRead(0));
    ledStates[i] = random(0, 2);
  }

}