#ifndef COIL_SENSOR_H_
#define COIL_SENSOR_H_

#include <Arduino.h>

/* initialization */
void CoilSensor_Init(void);

/* read the coil sensor value, returned: difference between L & R sensor
 * for a negative value the cable is more left than right.
 */
int16_t CoilSensor_ReadDeviation();

/* reads the coil sensor values,
   returns individual values for left an right sensor
   left value stored in element 0 of the array, right value in element 1
   Values reach from 0...4096 
*/
void CoilSensor_ReadIndividualValues(uint16_t *sensorValueArray);

#endif