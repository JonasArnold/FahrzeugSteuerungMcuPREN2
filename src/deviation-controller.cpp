#include "deviation-controller.h"
#include <Arduino.h>
#include "configuration.h"
#include "helpers.h"

// maximum sensor value (absolute)
const int maxSensorVal = 1700;   // maximum sensor value at ally
const int valMaxSteer = 1500;    // maximum steering required at this sensor value 200

// values for controller
const float Kp = 1.2f;
const float Td = 1.5f;  // 8
const float Ti = 0.1f;
const float u_min = -valMaxSteer;
const float u_max = valMaxSteer;
const float N = 5.0f;  // 5
float Tr = 0; 
float u_aw = 0;
float u_k = 0;
float u_k_sat = 0;
float e_k = 0;
float u_p = 0;
float u_d = 0;
float ad = 0;
float bd = 0;
float bi = 0;
float e_k_1 = 0;
float u_d_k_1 = 0;
float u_i_k_1 = 0;

void DeviationController_Init(void)
{
    u_k = 0;
    e_k = 0;
    u_p = 0;
    u_d = 0;
    e_k_1 = 0;
    u_d_k_1 = 0;
    u_i_k_1 = 0;
    u_aw = 0;

    // calculated constant parameters
    bi = Kp * T_MS / (2*Ti);
    ad = Td / (Td + N * T_MS);             // 0.07407407407407407407407407407407
    bd = (Kp * N * Td) / (Td + N * T_MS);  // 0.0462962962962962962962962962963
    Tr = sqrt(Ti * Td);
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

void DeviationController_CalcIndividualMotorPower(uint16_t requestedSpeed, uint16_t *sensorValuesArray, int16_t *motorControlArray)
{
    // requested speed zero => directly return zero and reset all stored values, do not calculate anything 
    if(requestedSpeed == 0)
    {
        motorControlArray[0] = 0;
        motorControlArray[1] = 0;
        u_i_k_1	= 0;
        u_d_k_1 = 0;
        e_k_1 = 0;
        return;
    }
    

    // calculate difference (deviation from middle) and sum
    int16_t difference = sensorValuesArray[0] - sensorValuesArray[1]; // difference > 0 ==> left of cable
    uint16_t sum = sensorValuesArray[0] + sensorValuesArray[1];
    int16_t speed = map(requestedSpeed, 0, 1000, 0, 255);

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
    u_k = u_p + u_d;// + u_i_k_1;

    // saturation
	u_k_sat = u_k;
	if(u_k_sat > u_max){
        u_k_sat = u_max;
    }
    else if(u_k_sat < u_min){
        u_k_sat = u_min;
    }

    u_aw = (T_MS/Tr) * (u_k_sat - u_k);
		
	// integral term (Trapezregel)
	u_i_k_1 = u_i_k_1 + bi * (e_k + e_k_1) + u_aw;

    // store old values
    e_k_1 = e_k;
    u_d_k_1 = u_d;

    // calculate motor power
    int rangeAbove = 255 - speed;
    int rangeBelow = speed;

    if(u_k > 0){
        motorControlArray[0] = speed - (((float)u_k/valMaxSteer) * rangeBelow);  
        motorControlArray[1] = speed + (((float)u_k/valMaxSteer) * rangeAbove);  
    }
    else{
        motorControlArray[0] = speed - (((float)u_k/valMaxSteer) * rangeAbove);  
        motorControlArray[1] = speed + (((float)u_k/valMaxSteer) * rangeBelow);  
    }

    // limit
    if(motorControlArray[0] < 0) { motorControlArray[0] = 0; }
    else if(motorControlArray[0] > 255) { motorControlArray[0] = 255; };
    if(motorControlArray[1] < 0) { motorControlArray[1] = 0; }
    else if(motorControlArray[1] > 255) { motorControlArray[1] = 255; };
    
    return;
}