#include "speedcontroller.h"
#include "motors.h"

void SpeedController_Init()
{

}

void SpeedController_Deinit()
{

}

int16_t SpeedController_CalcMotorPowerL(int setPointL)
{
    Motors_GetMMpSL();
    return 0;
}

int16_t SpeedController_CalcMotorPowerR(int setPointR)
{
    Motors_GetMMpSR();
    return 0;
}