#include "coil-sensor.h"

int sensor_left_pin = 36;

void CoilSensor_Init(void)
{
    pinMode(sensor_left_pin, INPUT);
    Serial.println("Coil Sensor initialized.");
}

uint16_t CoilSensor_Read()
{
    return analogRead(sensor_left_pin);
}