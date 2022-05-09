#include "motors.h"

// pins
const int motorL_forward_Pin = 18;
const int motorL_backward_Pin = 13;
const int motorL_enable_Pin = 27;
const int motorR_forward_Pin = 33;
const int motorR_backward_Pin = 12;
const int motorR_enable_Pin = 32;
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
int RPM_L, RPM_R;

// vars motor
const int forwardBackwardDelayTime = 50;  // [ms] delay to allow the H-bridge to stay at 0
unsigned long directionChangeMotorL = 0, directionChangeMotorR = 0;    // last time when motor  changed direction
MotorState stateMotorL = MotorState::Forward, stateMotorR = MotorState::Forward;       // current motor state
// PWM 
const int pwmFreq = 30000;
const int pwmChannelL = 0;
const int pwmChannelR = 1;
const int pwmResolution = 8;


// internal function declaration
void ISR_RPM_L(void);
void ISR_RPM_R(void);
static void Motor_WriteValues(int16_t left, int16_t right);

void Motors_Init()
{
    /* MOTOR SETUP */
    // make pins outputs
    pinMode(motorL_forward_Pin, OUTPUT);
    pinMode(motorL_backward_Pin, OUTPUT);
    pinMode(motorL_enable_Pin, OUTPUT);
    pinMode(motorR_forward_Pin, OUTPUT);
    pinMode(motorR_backward_Pin, OUTPUT);
    pinMode(motorR_enable_Pin, OUTPUT);

    // configure PWM
    ledcSetup(pwmChannelL, pwmFreq, pwmResolution);
    ledcSetup(pwmChannelR, pwmFreq, pwmResolution);

    // attach pins
    ledcAttachPin(motorL_enable_Pin, pwmChannelL);
    ledcAttachPin(motorR_enable_Pin, pwmChannelR);

    // write default duty cycles
    Motor_WriteValues(0, 0);
    
    delay(1000);

    /* RPM METER SETUP */
    // attach interrupts for rpm meters
    attachInterrupt(digitalPinToInterrupt(rpm_meter_L_Pin), ISR_RPM_L, RISING);
    attachInterrupt(digitalPinToInterrupt(rpm_meter_R_Pin), ISR_RPM_R, RISING);
}

void Motors_Deinit()
{

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

/* Writes motor pwm + forward/backward values
   each side accepts -255 to 255 value
*/
static void Motor_WriteValues(int16_t left, int16_t right)
{
    // limit values
    if(left < -255) { left = -255; }
    if(left > 255)  { left = 255;  }
    if(right < -255) { right = -255; }
    if(right > 255)  { right = 255;  }

    // write left values
    if(left == 0){
        digitalWrite(motorL_forward_Pin, LOW);
        digitalWrite(motorL_backward_Pin, LOW); 
        ledcWrite(pwmChannelL, left); 
    }
    else{
        if(left >= 0){
            digitalWrite(motorL_forward_Pin, HIGH);
            digitalWrite(motorL_backward_Pin, LOW); 
        }
        else if(left < 0){
            digitalWrite(motorL_forward_Pin, LOW);
            digitalWrite(motorL_backward_Pin, HIGH); 
        }
        ledcWrite(pwmChannelL, abs(left));   
    }
    
    // write right values
    if(right == 0){
        digitalWrite(motorR_forward_Pin, LOW);
        digitalWrite(motorR_backward_Pin, LOW); 
        ledcWrite(pwmChannelR, right); 
    }
    else{
        if(right >= 0){
            digitalWrite(motorR_forward_Pin, HIGH);
            digitalWrite(motorR_backward_Pin, LOW); 
        }
        else if(right < 0){
            digitalWrite(motorR_forward_Pin, LOW);
            digitalWrite(motorR_backward_Pin, HIGH); 
        }
        ledcWrite(pwmChannelR, abs(right));   
    }
}


/*  CalculateMotorSpeed => calculates the motors speed amount
 *  returns an int with the calculated speed from -255 to 255 
 */
static int16_t CalculateMotorSpeed(int16_t requestedAmount, unsigned long *lastDirectionChange, MotorState *currentState)
{
    int16_t dutyCycle = 0;  // default zero
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
                break; // return default
            }

            dutyCycle = requestedAmount;
            break;

        case MotorState::Backward:

            if(amount >= 0){
                *currentState = MotorState::SwitchingForward;
                *lastDirectionChange = now;
                break; // return default
            }

            dutyCycle = requestedAmount;
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

            break; // return default

        case MotorState::UnknownMotorState:
        default:
            /* code */
            *currentState = MotorState::Forward;
            break; // return default
    }

    return dutyCycle;
}

void Motors_Forward(int percent)
{
    int16_t dutyCycle = map(percent, 0, 100, 0, 255);
    Motor_WriteValues(dutyCycle, dutyCycle);
}


void Motors_ForwardAndSteering(uint8_t speed, int16_t steeringVal)
{
    int16_t leftAmount = speed, rightAmount = speed;

    leftAmount += steeringVal;
    rightAmount -= steeringVal;

    Motor_WriteValues(  CalculateMotorSpeed(leftAmount, &directionChangeMotorL, &stateMotorL),
                        CalculateMotorSpeed(rightAmount, &directionChangeMotorR, &stateMotorR));
}

void Motors_LeftRightIndividual(int16_t left, int16_t right)
{
    Motor_WriteValues(  CalculateMotorSpeed(left, &directionChangeMotorL, &stateMotorL),
                        CalculateMotorSpeed(right, &directionChangeMotorR, &stateMotorR));
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

