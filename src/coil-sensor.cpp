#include "coil-sensor.h"
#include "configuration.h"

const int sensor_left_pin = 36;
const int sensor_right_pin = 39;

// values for lowpass filter
const float KpLF = 1.0f;
const float TauLF = 0.5f;
float y_lf_l_1 = 0;         // last value of low pass filtered sensor signal LEFT
float y_lf_r_1 = 0;         // last value of low pass filtered sensor signal RIGHT
float y_lf_l = 0;           // current value of low pass filtered sensor signal LEFT
float y_lf_r = 0;           // current value of low pass filtered sensor signal LEFT

// declaration
static uint16_t readLeft(void);
static uint16_t readRight(void);

void CoilSensor_Init(void)
{
    // reset values
    y_lf_l_1 = 0;
    y_lf_r_1 = 0;
    y_lf_l = 0;
    y_lf_r = 0;

    pinMode(sensor_left_pin, INPUT);
    pinMode(sensor_right_pin, INPUT);
    Serial.println("Coil Sensor initialized.");
}

int16_t CoilSensor_ReadDeviation()
{
    uint16_t leftVal = readLeft();
    uint16_t rightVal = readRight();

    int16_t difference = (int16_t)leftVal - (int16_t)rightVal;

    return difference;
}

void CoilSensor_ReadIndividualValues(uint16_t *sensorValueArray)
{
    /* PT1 lowpass filter on sensor values */
    // calculate current values
    y_lf_l = y_lf_l_1 + (KpLF * (float)readLeft() - y_lf_l_1) * (T_MS/TauLF);
    y_lf_r = y_lf_r_1 + (KpLF * (float)readRight() - y_lf_r_1) * (T_MS/TauLF);
    // store old values
    y_lf_l_1 = y_lf_l;
    y_lf_r_1 = y_lf_r;

    sensorValueArray[0] = readLeft();
    sensorValueArray[1] = readRight();
}

static uint16_t readLeft(void){
    return analogRead(sensor_left_pin);
}

static uint16_t readRight(void){
    return analogRead(sensor_right_pin);
}