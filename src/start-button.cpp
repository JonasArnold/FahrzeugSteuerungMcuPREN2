#include <Arduino.h>
#include "start-button.h"

// pins
const int start_Pin = 15;
bool startButtonEvent = false;

static void ISR_BUTTON_PRESSED(void)
{
    // if button is pressed
    if(StartButton_GetState()){
        startButtonEvent = true; // set event
    }
}

void StartButton_Init()
{
    pinMode(start_Pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(start_Pin), ISR_BUTTON_PRESSED, FALLING);
}

bool StartButton_GetEventHappened(void)
{
    if (startButtonEvent == true)
    {
        startButtonEvent = false;  // reset event
        return true;
    }
    return false;
}

bool StartButton_GetState(void)
{
    // if button is pressed
    if(digitalRead(start_Pin) == LOW)
    {
        return true;
    }
    return false;
}