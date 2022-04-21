#include <Arduino.h>
#include "remote-control.h"
#include "helpers.h"
#include "configuration.h"

#define REMOTE_CONTROL_CONNECTED (throttleRecieved && steeringRecieved)

const int throttlePin = 25;
const int steeringPin = 26;
bool throttleRecieved = false, steeringRecieved = false;

void RemoteControl_Init()
{
#ifdef REMOTE_CONTROL_ENABLED
    pinMode(throttlePin, INPUT);
    pinMode(steeringPin, INPUT);
    throttleRecieved = false;
    steeringRecieved = false;
#endif
}

uint8_t RemoteControl_GetThrottle()
{
#ifdef REMOTE_CONTROL_ENABLED
    unsigned long throttle = pulseIn(throttlePin, HIGH);
    Helpers_SerialPrintLnAndVal("Throttle: ", throttle);
    uint8_t mappedThrottle = map(throttle , 1080, 1930, 0, 255 );
    
    // only return if remote is initialized
    if(REMOTE_CONTROL_CONNECTED)
    {
        return mappedThrottle;
    }

    // throttle initialized to zero
    if(mappedThrottle == 0)
    {
        throttleRecieved = true;
    }
#endif
    return 0;
}


int8_t RemoteControl_GetSteering()
{
#ifdef REMOTE_CONTROL_ENABLED
    unsigned long steering = pulseIn(steeringPin, HIGH);
    Helpers_SerialPrintLnAndVal("Steering: ", steering);
    int8_t mappedSteering = map( steering, 1080, 1930, -127, 127 );

    // only return if remote is initialized
    if(REMOTE_CONTROL_CONNECTED)
    {
        return mappedSteering;
    }

    // steering initialized to zero
    if(mappedSteering == 0)
    {
        steeringRecieved = true;
    }
#endif
    return 0;
}

bool RemoteControl_GetConnectedState()
{
    return REMOTE_CONTROL_CONNECTED;
}