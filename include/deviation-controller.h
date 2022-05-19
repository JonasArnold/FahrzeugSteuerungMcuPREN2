#ifndef DEVIATION_CONTROLLER_H_
#define DEVIATION_CONTROLLER_H_

#include <Arduino.h>

/* initialization */
void DeviationController_Init(void);
/* de-initialization */
void DeviationController_Deinit(void);

/* debug getters */
int16_t GetUk();
int16_t GetUd();
int16_t GetUp();

/* Calculate steering value according to deviation 
 * out value: -127 ... 127 
 */
int16_t DeviationController_CalcSteering(int16_t deviation);

/* Calculate motor power of left and right motor
   according to left and right sensor value
   requested speed 0 ... 255
   sensor left and right values reach from 0 ... 4096
   motor left and right values shall reach from -255 ... 255
 */
void DeviationController_CalcIndividualMotorPower(uint8_t requestedSpeed, uint16_t *sensorValuesArray, int16_t *motorControlArray);

#endif