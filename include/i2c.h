#ifndef I2C_H_
#define I2C_H_

#include <Arduino.h>

typedef enum
{
    None = 0, 
    SpeedDown = 1, 
    SpeedUp = 2, 
    Stop = 3
} Command;

void I2C_Init(void);

Command I2C_Handle(void);

void I2C_SetDeviceState(byte);
void I2C_SetBatteryLevel(byte);
void I2C_SetSetSpeed(byte);

#endif