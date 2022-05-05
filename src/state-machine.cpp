#include <Arduino.h>
#include "state-machine.h"
#include "configuration.h"

static DeviceState currentState;

void StateMachine_Init(void){
    currentState = UnknownDeviceState;
}

/* module de-initialization */
void StateMachine_Deinit(void){
    currentState = UnknownDeviceState;
}

/* set current state */
void StateMachine_SetCurrentState(DeviceState newState){
    currentState == newState;
}

/* get current state */
DeviceState StateMachine_GetCurrentState(){
    return currentState;
}

int StateMachine_GetSpeedApropriateToState(){
    switch (currentState)
    {
        case NormSpeed:
            return SPEED_FAST;
        case RedSpeed:
            return SPEED_SLOW;
        case UnknownDeviceState: 
        case Ready:  
        case Stopped: 
        default:
            return SPEED_HALT;
    }
}