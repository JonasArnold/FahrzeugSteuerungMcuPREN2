#include <Arduino.h>
#include "start-button.h"

// pins
const int start_Pin = 15;

void StartButton_Init()
{
    pinMode(start_Pin, INPUT);
}

bool StartButton_GetState()
{
    if (digitalRead(start_Pin)==LOW)
    {
        return true;
    }
    return false;
}