#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <Arduino.h>

typedef enum {
    UnknownDeviceState = 0, 
    Ready = 1, 
    NormSpeed = 2, 
    RedSpeed = 3, 
    Stopped = 4 
} DeviceState;


/* module initialization */
void StateMachine_Init(void);

/* module de-initialization */
void StateMachine_Deinit(void);

/* set current state */
void StateMachine_SetCurrentState(DeviceState newState);

/* get current state */
DeviceState StateMachine_GetCurrentState();

/* returns the motor speed according to the device state */
int StateMachine_GetSpeedApropriateToState();


#endif