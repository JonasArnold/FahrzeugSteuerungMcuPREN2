#ifndef MOTORS_H_
#define MOTORS_H_

#include <Arduino.h>

/* initialization */
void Motors_Init(void);
/* de-initialization */
void Motors_Deinit(void);
/* handle internal stuff, function to call in super loop (peridocally) */
void Motors_Handle(void);


/* move device forward, int = rpm */ 
void Motors_Forward(int);

/* move device forward with steering capability
   speed: 0...255 speed
   steeringVal: -127 ... +127 (amount left or right), negative is left
*/
void Motors_ForwardAndSteering(uint8_t speed, int8_t steeringVal);

/* get left motors current rpm */
int16_t Motors_GetRpmL();
/* get right motors current rpm */
int16_t Motors_GetRpmR();

/* get left motors current millimeters per second */
int16_t Motors_GetMMpSL();
/* get right motors current millimeters per second */
int16_t Motors_GetMMpSR();

#endif