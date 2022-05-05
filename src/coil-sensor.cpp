#include "coil-sensor.h"

const int sensor_left_pin = 36;
const int sensor_right_pin = 39;

void CoilSensor_Init(void)
{
    pinMode(sensor_left_pin, INPUT);
    pinMode(sensor_right_pin, INPUT);
    Serial.println("Coil Sensor initialized.");
}

int16_t CoilSensor_ReadDeviation()
{
    uint16_t leftVal = analogRead(sensor_left_pin);
    uint16_t rightVal = analogRead(sensor_right_pin);

    int16_t difference = (int16_t)leftVal - (int16_t)rightVal;

    return difference;
}

void CoilSensor_ReadIndividualValues(uint16_t *sensorValueArray)
{
    sensorValueArray[0] = analogRead(sensor_left_pin);
    sensorValueArray[1] = analogRead(sensor_right_pin);
}