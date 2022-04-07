#ifndef BATTERY_MONITORING_H_
#define BATTERY_MONITORING_H_

#include <Arduino.h>

/* initialization */
void BatteryMonitoring_Init();

/* returns battery voltage in millivolts  */
double BatteryMonitoring_GetBatteryVoltage();

/* get percentage of battery */
uint8_t BatteryMonitoring_GetPercent();

/* returns true if a critical charge is reached, otherwise false */
bool BatteryMonitoring_CriticalCharge();

#endif