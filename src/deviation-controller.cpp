#include "deviation-controller.h"
#include <Arduino.h>
#include "configuration.h"

const float Kp = 2/15.74f;


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
    if (steering > 255)  { steering = 255;  }
    if (steering < -255) { steering = -255; }
    return steering;
}

void DeviationController_CalcIndividualMotorPower(uint8_t requestedSpeed, uint16_t *sensorValuesArray, int16_t *motorControlArray)
{
    int16_t difference = (int16_t)sensorValuesArray[0] - (int16_t)sensorValuesArray[1];
    uint16_t absDifference = abs(difference);
    uint16_t sum = sensorValuesArray[0] + sensorValuesArray[1];
    uint8_t speed = requestedSpeed;

    // requested speed = 0
    if(requestedSpeed == 0)
    {
        motorControlArray[0] = 0;
        motorControlArray[1] = 0;
        return;
    }

    // cable lost
    if(sum < 500)
    {
        // TODO improve find back to path
        motorControlArray[0] = -80;
        motorControlArray[1] = -80;
        return;
    }

    // big deviation => reduce speed, the bigger the deviation
    if(absDifference > 600)
    {
        speed -= ((absDifference-600) / 4); // e.g. at difference of 1000 => -100 speed
    }


    int16_t steering = -(Kp * (float)difference);
    motorControlArray[0] = speed + steering; 
    motorControlArray[1] = speed - steering;
}