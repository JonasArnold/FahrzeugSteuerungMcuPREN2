#include <Arduino.h>
#include "state-machine.h"
#include "configuration.h"
#include "helpers.h"

static DeviceState currentState;

void StateMachine_Init(void){
    StateMachine_SetCurrentState(UnknownDeviceState);
}

/* module de-initialization */
void StateMachine_Deinit(void){
    StateMachine_SetCurrentState(UnknownDeviceState);
}

/* set current state */
void StateMachine_SetCurrentState(DeviceState newState){
    currentState = newState;
    Helpers_SerialPrintLnAndVal("State changed to:", currentState);
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