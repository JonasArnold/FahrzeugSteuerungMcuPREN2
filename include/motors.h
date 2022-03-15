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

#endif