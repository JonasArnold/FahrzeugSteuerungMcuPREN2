#include <Arduino.h>
#include "battery-monitoring.h"
#include "helpers.h"

/* PINS */
const int measuring_pin = 14;

/* CONSTANTS */
const double scalingFactor = 2.7447;                // = 1/(47/129)
const double milliVoltsPerAnalogVal = 0.080566;     // = (330/4096);
const int millisBetweenBatteryMeasurement = 2000;   // milliseconds between battery measurement

/* VARIABLES */
static double batteryVoltage;       // stores battery voltage in millivolts
static long lastBatteryMeasurement; // stores last battery measurement millis

void BatteryMonitoring_Init()
{
    pinMode(measuring_pin, INPUT);
    batteryVoltage = 0;
    lastBatteryMeasurement = 0;
}

/* returns battery voltage in millivolts  */
double BatteryMonitoring_GetBatteryVoltage()
{
    unsigned long now = millis();
    // request battery voltage from sensor if measurement is overdue
    if(now - lastBatteryMeasurement > millisBetweenBatteryMeasurement)
    {
        double voltage  = analogRead(measuring_pin) * milliVoltsPerAnalogVal; // get voltage in millivolts
        voltage = voltage * scalingFactor; // multiply by voltage divider constant
        Helpers_SerialPrintLnAndVal("Battery voltage measured:", (uint16_t)voltage);
        batteryVoltage = voltage;  // set stored value

        // set now as last measurement time 
        lastBatteryMeasurement = now;
    }

    return batteryVoltage; 
}

uint8_t BatteryMonitoring_GetPercent()
{
    long battery_mv = (long)BatteryMonitoring_GetBatteryVoltage();
    if(battery_mv <= 720)
    {
        return 0;
    }
    if(battery_mv >= 840)
    {
        return 100;
    }
    
    return (uint8_t)map(battery_mv, 720, 840, 1, 99);
}

bool BatteryMonitoring_CriticalCharge()
{
    if(BatteryMonitoring_GetBatteryVoltage() < 740.0)
    {
        return true;
    }
    return false;
}
