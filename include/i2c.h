#ifndef I2C_H_
#define I2C_H_

#include <Arduino.h>

byte state;
byte batteryLevel;
byte speed;

void I2C_Init(void);

void receiveData(int byteCount);

void sendData(void);

int I2C_Handle(void);

#endif