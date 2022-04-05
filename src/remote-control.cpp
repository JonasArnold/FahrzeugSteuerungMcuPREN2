#include <Arduino.h>
#include "remote-control.h"
#include "helpers.h"


const int throttlePin = 25;
const int steeringPin = 26;

void RemoteControl_Init()
{
    pinMode(throttlePin, INPUT);
    pinMode(steeringPin, INPUT);
}

uint8_t RemoteControl_GetThrottle()
{
    unsigned long throttle = pulseIn(throttlePin, HIGH);
    Helpers_SerialPrintLnAndVal("Throttle: ", throttle);
    return map(throttle , 1080, 1930, 0, 255 );
}


int8_t RemoteControl_GetSteering()
{
    unsigned long steering = pulseIn(steeringPin, HIGH);
    Helpers_SerialPrintLnAndVal("Steering: ", steering);
    return map( steering, 1080, 1930, -127, 127 );
}