#ifndef DEVIATION_CONTROLLER_H_
#define DEVIATION_CONTROLLER_H_

#include <Arduino.h>

/* initialization */
void DeviationController_Init(void);
/* de-initialization */
void DeviationController_Deinit(void);

/* Calculate steering value according to deviation 
 * out value: -127 ... 127 
 */
int16_t DeviationController_CalcSteering(int16_t deviation);

#endif