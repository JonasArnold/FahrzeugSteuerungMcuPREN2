#ifndef I2C_H_
#define I2C_H_

#include <Arduino.h>

void I2C_Init(void);

int I2C_Handle(void);

int get_state(void);
void set_state(int);

int get_batteryLevel(void);
void set_batteryLevel(int);

int get_speed(void);
void set_speed(int);

#endif