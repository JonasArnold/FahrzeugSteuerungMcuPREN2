#ifndef I2C_H_
#define I2C_H_

#include <Arduino.h>

enum DeviceState{Unknown, Ready, NormSpeed, RedSpeed, Stopped};

enum Command
{
    None = 0, 
    SpeedDown = 1, 
    SpeedUp = 2, 
    Stop = 3
};

void I2C_Init(void);

int I2C_Handle(void);

DeviceState get_state(void);
void set_state(DeviceState);

int get_batteryLevel(void);
void set_batteryLevel(int);

int get_speed(void);
void set_speed(int);

#endif