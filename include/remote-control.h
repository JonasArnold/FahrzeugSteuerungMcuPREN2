#ifndef REMOTE_CONTROL_H_
#define REMOTE_CONTROL_H_

#include <Arduino.h>

void RemoteControl_Init();

uint8_t RemoteControl_GetThrottle();
int16_t RemoteControl_GetSteering();
bool RemoteControl_GetConnectedState();

#endif