#ifndef COIL_SENSOR_H_
#define COIL_SENSOR_H_

#include <Arduino.h>

/* initialization */
void CoilSensor_Init(void);

/* read the coil sensor value, returned: difference between L & R sensor
 * for a negative value the cable is more left than right.
 */
int16_t CoilSensor_Read();

#endif