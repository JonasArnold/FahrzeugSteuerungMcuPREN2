#include "motors.h"

// define min / max microseconds on time for servo signal
#define MIN_US 1000
#define MAX_US 2000
#define FREQUENCY 50

// create servo objects 
Servo motorL;
Servo motorR;
int motorL_Pin = 12;
int motorR_Pin = 13;

void Init_Motors()
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
}

void Deinit_Motors()
{
    // detach motors
    motorL.detach();
    motorR.detach();
}

void Forward_Motors(int percent)
{
    motorL.write(map(percent, 0, 100, 98, 110));
    motorR.write(map(percent, 0, 100, 98, 110));
}

