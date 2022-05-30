#include "deviation-controller.h"
#include <Arduino.h>
#include "configuration.h"
#include "helpers.h"

// cycle time
const float T_ms = 0.5f;

// values for lowpass filter
const float KpLF = 1.0f;
const float TauLF = 2.0f;
float y_lf_l_1 = 0;         // last value of low pass filtered sensor signal LEFT
float y_lf_r_1 = 0;         // last value of low pass filtered sensor signal RIGHT
float y_lf_l = 0;           // current value of low pass filtered sensor signal LEFT
float y_lf_r = 0;           // current value of low pass filtered sensor signal LEFT

// values for controller
const float Kp = 0.125f;
const float Td = 0.3f;
const float N = 5.0f;
float u_k = 0;
float e_k = 0;
float u_p = 0;
float u_d = 0;
float ad = 0;
float bd = 0;
float e_k_1 = 0;
float u_d_k_1 = 0;

void DeviationController_Init(void)
{
    // reset values
    y_lf_l_1 = 0;
    y_lf_r_1 = 0;
    y_lf_l = 0;
    y_lf_r = 0;
    u_k = 0;
    e_k = 0;
    u_p = 0;
    u_d = 0;
    e_k_1 = 0;
    u_d_k_1 = 0;

    // calculated constant parameters
    ad = Td / (Td + N * T_ms);             // 0.07407407407407407407407407407407
    bd = (Kp * N * Td) / (Td + N * T_ms);  // 0.0462962962962962962962962962963
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

int16_t GetUk()
{
    return (int16_t)u_k;
}
int16_t GetUp()
{
    return (int16_t)u_p;
}
int16_t GetUd()
{
    return (int16_t)u_d;
}

void DeviationController_CalcIndividualMotorPower(uint8_t requestedSpeed, uint16_t *sensorValuesArray, int16_t *motorControlArray)
{
    // requested speed zero => directly return zero and reset all stored values, do not calculate anything 
    if(requestedSpeed == 0)
    {
        motorControlArray[0] = 0;
        motorControlArray[1] = 0;
        y_lf_l_1 = 0;
        y_lf_r_1 = 0;
        u_d_k_1 = 0;
        e_k_1 = 0;
        return;
    }
    

    /* PT1 lowpass filter on sensor values */
    // calculate current values
    y_lf_l = y_lf_l_1 + (KpLF * (float)sensorValuesArray[0] - y_lf_l_1) * (T_ms/TauLF);
    y_lf_r = y_lf_r_1 + (KpLF * (float)sensorValuesArray[1] - y_lf_r_1) * (T_ms/TauLF);
    // store old values
    y_lf_l_1 = y_lf_l;
    y_lf_r_1 = y_lf_r;

    // calculate difference (deviation from middle) and sum
    int16_t difference = y_lf_l - y_lf_r; // difference > 0 ==> left of cable
    uint16_t sum = y_lf_l + y_lf_r;
    int16_t speed = (int16_t)requestedSpeed;

    // cable lost => drive way back
    if(sum < 500)
    {
        // TODO improve find back to path
        if(difference > 0){  // on the left side of the cable
            motorControlArray[0] = 30;
            motorControlArray[1] = 100;
        }
        else{  // on the right side of the cable
            motorControlArray[0] = 100;
            motorControlArray[1] = 30;
        }
        return;
    }

    /* PD controller to calculate steering */
    // compute the error from the low pass filtered signal
    e_k = difference;       // reference = 0 
	// proportinal term
	u_p = Kp * e_k;
    // derivative term with filter
	u_d	= ad * u_d_k_1 + bd * (e_k - e_k_1);
    // total of PD values
    u_k = u_p + u_d;
    // store old values
    e_k_1 = e_k;
    u_d_k_1 = u_d;

    /* LIMITER */
    // calculate motor power, limit motor values  
    motorControlArray[0] = speed * (1.0 + (u_k/100.0  * 0.4));
    if(motorControlArray[0] < 50) { motorControlArray[0] = 50; }
    else if(motorControlArray[0] > 255) { motorControlArray[0] = 255; };
    motorControlArray[1] = speed * (1 - (u_k/100.0  * 0.4));
    if(motorControlArray[1] < 50) { motorControlArray[1] = 50; }
    else if(motorControlArray[1] > 255) { motorControlArray[1] = 255; };
    
    return;
}