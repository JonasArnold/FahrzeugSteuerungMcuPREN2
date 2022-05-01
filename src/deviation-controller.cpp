#include "deviation-controller.h"
#include <Arduino.h>
#include "configuration.h"

const float Kp = 1/15.74f;


void DeviationController_Init(void)
{
    /* NOP */
}


void DeviationController_Deinit(void)
{
    /* NOP */
}

// negative deviation = being more left 
// negative steering = steering left
int16_t DeviationController_CalcSteering(int16_t deviation)
{
    int16_t steering = -(Kp * (float)deviation);
    if (steering > 127)  { steering = 127;  }
    if (steering < -127) { steering = -127; }
    return steering;
}