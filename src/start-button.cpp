#include <Arduino.h>
#include "start-button.h"

// pins
const int start_Pin = 15;

void StartButton_Init()
{
    pinMode(start_Pin, INPUT);
}

int StartButton_Handle()
{
    if (digitalRead(start_Pin)==LOW)
    {
        return 1;
    }
    return 0;
}