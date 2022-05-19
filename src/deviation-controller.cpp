#include "deviation-controller.h"
#include <Arduino.h>
#include "configuration.h"

const float T_ms = 0.5f;

// values for lowpass filter
const float KpLF = 1.0f;
const float TauLF = 1.0f;
float y_lf_1 = 0;
float y_lf = 0;

// values for controller
const float Kp = 1.0f/10.0f;
const float Td = 2.0f;
const float N = 10.0f;
float u_k = 0;
float e_k = 0;
float ref = 0;
float u_p = 0;
float u_d = 0;
float ad = 0;
float bd = 0;
float e_k_1 = 0;
float u_d_k_1 = 0;

void DeviationController_Init(void)
{
    // reset values
    y_lf_1 = 0;
    y_lf = 0;
    u_k = 0;
    e_k = 0;
    ref = 0;
    u_p = 0;
    u_d = 0;
    e_k_1 = 0;
    u_d_k_1 = 0;

    // calculated constant parameters
    ad = Td / (Td + N * T_ms);;
    bd = (Kp * N * Td) / (Td + N * T_ms);
}


void DeviationController_Deinit(void)
{
    /* NOP */
}

// negative deviation = being more left 
// negative steering = steering left
int16_t DeviationController_CalcSteering(int16_t deviation)
{
    int16_t steering = -(Kp * (float)deviation);
    if (steering > 255)  { steering = 255;  }
    if (steering < -255) { steering = -255; }
    return steering;
}

void DeviationController_CalcIndividualMotorPower(uint8_t requestedSpeed, uint16_t *sensorValuesArray, int16_t *motorControlArray)
{
    int16_t difference = (int16_t)sensorValuesArray[0] - (int16_t)sensorValuesArray[1];  // difference > 0 ==> left of cable
    //uint16_t absDifference = abs(difference);
    //uint16_t sum = sensorValuesArray[0] + sensorValuesArray[1];
    int16_t speed = (int16_t)requestedSpeed;

    // requested speed = 0
    if(requestedSpeed == 0)
    {
        motorControlArray[0] = 0;
        motorControlArray[1] = 0;
        return;
    }

    // lowpass filter
    y_lf = y_lf_1 + (KpLF * difference - y_lf_1) * (T_ms/TauLF);


    // PD controller
    // compute the error from the low pass filtered signal
    ref = requestedSpeed;
	e_k = ref - y_lf;
		
	// proportinal term
	u_p = Kp * e_k;

    // derivative term with filter
	u_d	= ad * u_d_k_1 + bd * (e_k - e_k_1);

    // total command
    u_k = u_p + u_d;

    // store old values
    y_lf_1 = y_lf;
    e_k_1 = e_k;
    u_d_k_1 = u_d;


    // cable lost
 /*   if(sum < 500)
    {
        // TODO improve find back to path
        if(difference > 0){  // on the left side of the cable
            motorControlArray[0] = 70;
            motorControlArray[1] = 0;
        }
        else{  // on the right side of the cable
            motorControlArray[0] = 0;
            motorControlArray[1] = 70;
        }
        return;
    }
*/

    // big deviation => reduce speed, the bigger the deviation
/*
    if(absDifference > 400)
    {
        speed -= ((absDifference-400) / 4); // e.g. at difference of 1000 => -100 speed
    }
*/


    int16_t steering = (int16_t)u_k;
    /*int16_t baseSpeed = speed/5;  // base speed = speed minimum, always drive this speed on both sides
    int16_t restOfSpeed = speed-baseSpeed;
    motorControlArray[0] = baseSpeed + (restOfSpeed + steering); 
    motorControlArray[1] = baseSpeed + (restOfSpeed - steering);
    */
    
    motorControlArray[0] = speed + steering; 
    motorControlArray[1] = speed - steering;
    
    /* concept with mutliplication, to become speed independent, does not work since steering can get negative */
    // catch zero division
    /*if(steering == 0){
        motorControlArray[0] = speed;
        motorControlArray[1] = speed;
        return;
    }
    motorControlArray[0] = speed * (steering/255); 
    motorControlArray[1] = speed / (steering/255);
    */
    return;
}