#ifndef COIL_SENSOR_H_
#define COIL_SENSOR_H_

#include <Arduino.h>

/* initialization */
void CoilSensor_Init(void);

/* read the coil sensor value, returned: difference between L & R sensor */
uint16_t CoilSensor_Read();

#endif