#ifndef _MOTORS_H_
#define _MOTORS_H_

#include <ESP32Servo.h>

void Init_Motors(void);
void Deinit_Motors(void);
void Handle_Motors(void);

void Forward_Motors(int);

#endif