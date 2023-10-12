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

int BUTTONS[numLeds] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6};
int LEDS[numLeds] = {LED1, LED2, LED3, LED4, LED5, LED6};


// put function declarations here:
// int myFunction(int, int);

void setup() {

  for (int i = 0; i < numLeds; i++)
  {
    pinMode(BUTTONS[i], INPUT_PULLUP);
    pinMode(LEDS[i], OUTPUT);
  }

}

void loop() {

  for (int i = 0; i < numLeds; i++)
  {
    bool buttonPressed = digitalRead(BUTTONS[i]);
    digitalWrite(LEDS[i], !buttonPressed);
  }

}
