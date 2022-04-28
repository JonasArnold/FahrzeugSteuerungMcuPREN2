#ifndef SPEEDCONTROLLER_H_
#define SPEEDCONTROLLER_H_

#include <Arduino.h>

/* initialization */
void SpeedController_Init(void);
/* de-initialization */
void SpeedController_Deinit(void);

/* Calculate amount of motor power to provide according to set point  
 * setPointL: [mm/s]
 */
int16_t SpeedController_CalcMotorPowerL(int setPointL);

/* Calculate amount of motor power to provide according to set point  
 * setPointR: [mm/s]
 */
int16_t SpeedController_CalcMotorPowerR(int setPointR);

#endif