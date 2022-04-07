#include "motors.h"
#include <ESP32Servo.h>

// define min / max microseconds on time for servo signal
#define MIN_US 1000
#define MAX_US 2000
#define FREQUENCY 50

// create servo objects 
Servo motorL;
Servo motorR;

// pins
const int motorL_Pin = 12;
const int motorR_Pin = 13;
const int rpm_meter_L_Pin = 0;
const int rpm_meter_R_Pin = 2;

// vars
unsigned int flank_count_l, flank_count_r;
const int factor = 60/25;                   // (60 secs / min) *  [Amount of flanks per full revolution]
const int millisBetweenRpmCount = 1000;     // calculate rpm every x [ms]
unsigned long lastRpmCalculation = 0;       // stores last rpm calculation millis

int RPM_L, RPM_R;

// internal function declaration
void ISR_RPM_L(void);
void ISR_RPM_R(void);

void Motors_Init()
{
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    
    // set frequency
    motorL.setPeriodHertz(FREQUENCY);
	motorR.setPeriodHertz(FREQUENCY);

    // attach motors
    motorL.attach(motorL_Pin, MIN_US, MAX_US);
	motorR.attach(motorR_Pin, MIN_US, MAX_US);

    // set to 0 and wait 5 seconds (let ESC initialize)
    motorL.write(90);
    motorR.write(90);
    delay(5000);

    // attach interrupts for rpm meters
    attachInterrupt(digitalPinToInterrupt(rpm_meter_L_Pin), ISR_RPM_L, RISING);
    attachInterrupt(digitalPinToInterrupt(rpm_meter_R_Pin), ISR_RPM_R, RISING);
}

void Motors_Deinit()
{
    // detach motors
    motorL.detach();
    motorR.detach();
}

void Motors_Handle()
{
    unsigned long now = millis();
    // perform rpm calculation if it is overdue
    if(now - lastRpmCalculation > millisBetweenRpmCount)
    {
        RPM_L = flank_count_l * factor;
        RPM_R = flank_count_r * factor;
        flank_count_l = 0;
        flank_count_r = 0;

        // set now as last rpm calculation time
        lastRpmCalculation = now;
    }
}


/* Functions */

void Motors_Forward(int percent)
{
    motorL.write(map(percent, 0, 100, 98, 110));
    motorR.write(map(percent, 0, 100, 98, 110));
}


void Motors_ForwardAndSteering(uint8_t speed, int8_t steeringVal)
{
    uint8_t leftAmount = speed, rightAmount = speed;

    leftAmount += 2*steeringVal;
    rightAmount -= 2*steeringVal;

    motorL.write(map(leftAmount, 0, 255, 98, 180));
    motorR.write(map(rightAmount, 0, 255, 98, 180));
}


/* ISR */
void ISR_RPM_L(void)
{
    flank_count_l++;
}
void ISR_RPM_R(void)
{
    flank_count_r++;
}