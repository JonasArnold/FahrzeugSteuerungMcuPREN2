#include "motors.h"
#include <ESP32Servo.h>

// define min / max microseconds on time for servo signal
#define MIN_US 1000
#define MAX_US 2000
#define DEFAULT_US  1500
#define FREQUENCY 50

// create servo objects 
Servo motorL;
Servo motorR;

// pins
const int motorL_Pin = 12;
const int motorR_Pin = 13;
const int rpm_meter_L_Pin = 16;
const int rpm_meter_R_Pin = 2;

// vars turn counting
unsigned int flank_count_l, flank_count_r;
const int factor = 60/25;                   // (60 secs / min) *  [Amount of flanks per full revolution]
const int millisBetweenRpmCount = 1000;     // calculate rpm every x [ms]
unsigned long lastRpmCalculation = 0;       // stores last rpm calculation millis
const int wheelPerimeter = 408;             // [mm] perimeter of the wheel
const int wheelRadius = 65;                 // [mm] radius of wheel
const float transmissionRatio = 1.8f;       // factor of transmission between motor and wheel

// vars motor
const int forwardBackwardDelayTime = 1000;  // [ms] delay to allow esc to switch from forward to backward rotation
unsigned long directionChangeMotorL = 0, directionChangeMotorR = 0;    // last time when motor  changed direction
MotorState stateMotorL = MotorState::Forward, stateMotorR = MotorState::Forward;       // current motor state

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
    motorL.write(DEFAULT_US);
    motorR.write(DEFAULT_US);
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

/*  CalculateMotorSpeed => calculates the motors speed amount
 *  returns an int with the calculated speed in microseconds (for servo control)
 */
static int CalculateMotorSpeed(int16_t requestedAmount, unsigned long *lastDirectionChange, MotorState *currentState)
{
    int microseconds = DEFAULT_US;  // default neutral
    int16_t amount = requestedAmount;  // copy value
    unsigned long now = millis();

    // limit values
    if(amount < -255) { amount = -255; }
    if(amount > 255)  { amount = 255;  }

    switch (*currentState)
    {
        case MotorState::Forward:
            
            if(amount < 0){
                *currentState = MotorState::SwitchingBackward;
                *lastDirectionChange = now;
                break; // return default us
            }

            microseconds = map(amount, 0, 255, 1530, 1950);
            break;

        case MotorState::Backward:

            if(amount >= 0){
                *currentState = MotorState::SwitchingForward;
                *lastDirectionChange = now;
                break; // return default us
            }

            // double amount since only half the power is provided by the ESC in Backward mode
            amount = amount * 2;  
            // limit value again
            if(amount < -255) { amount = -255; }

            microseconds = map(amount, -255, 0, 1050, 1470);
            break;

        case MotorState::SwitchingBackward:
        case MotorState::SwitchingForward:

            // proceed with other direction if waiting time is done
            if(now - *lastDirectionChange > forwardBackwardDelayTime){
                if(*currentState == MotorState::SwitchingBackward){
                    *currentState = MotorState::Backward;
                }
                if(*currentState == MotorState::SwitchingForward){
                    *currentState = MotorState::Forward;
                }
            }

            break; // return default us

        case MotorState::UnknownMotorState:
        default:
            /* code */
            *currentState = MotorState::Forward;
            break; // return default us
    }

    return microseconds;
}

void Motors_Forward(int percent)
{
    motorL.write(map(percent, 0, 100, 98, 170));
    motorR.write(map(percent, 0, 100, 98, 170));
}


void Motors_ForwardAndSteering(uint8_t speed, int16_t steeringVal)
{
    int16_t leftAmount = speed, rightAmount = speed;

    leftAmount += steeringVal;
    rightAmount -= steeringVal;

    motorL.writeMicroseconds(CalculateMotorSpeed(leftAmount, &directionChangeMotorL, &stateMotorL));
    motorR.writeMicroseconds(CalculateMotorSpeed(rightAmount, &directionChangeMotorR, &stateMotorR));
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


int16_t Motors_GetRpmL()
{
    return RPM_L;
}

int16_t Motors_GetRpmR()
{
    return RPM_R;
}

int16_t ConvertRpmToMMpS(int16_t rpm)
{
    return (int16_t)(rpm / transmissionRatio) * ((float)wheelRadius / (60.0f));
}

int16_t Motors_GetMMpSL()
{
    int16_t mps = ConvertRpmToMMpS(RPM_L);
    if(stateMotorL == MotorState::Backward)
    {
        mps = -1 * mps;
    }
    return mps;
}

int16_t Motors_GetMMpSR()
{
    int16_t mps = ConvertRpmToMMpS(RPM_R);
    if(stateMotorR == MotorState::Backward)
    {
        mps = -1 * mps;
    }
    return mps;
}

