#include "coil-sensor.h"

int sensor_left_pin = 36;

void Init_CoilSensor(void)
{
    pinMode(sensor_left_pin, INPUT);
    Serial.println("Coil Sensor initialized.");
}


uint16_t Read_CoilSensor()
{
    return analogRead(sensor_left_pin);
}