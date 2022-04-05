#include <Arduino.h>
#include "helpers.h"
#include "display.h"
#include "coil-sensor.h"
#include "motors.h"
#include "i2c.h"
#include "remote-control.h"

uint16_t sensorValue = 0;
uint8_t speedVal;
int8_t steeringVal;
uint8_t driveCommand;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Display_Init();
  CoilSensor_Init();

#ifdef COMMUNICATION_ENABLED
  Display_ShowText(15, 15, String("Initializing I2C..."));
  I2C_Init();
#endif

  RemoteControl_Init();

  Display_Clear();
  Display_ShowText(15, 15, String("Initializing motors..."));
  Motors_Init();

  Display_Clear();
  Display_ShowText(15, 15, String("Init done"));
  delay(2000);
  Display_Clear();
  Display_SetupBase();

  // TEST_I2C VAR
  set_state(Ready); set_batteryLevel(0); set_speed(1000);
}

void loop() {

#ifdef COMMUNICATION_ENABLED
  // handle all periodic stuff
  driveCommand = I2C_Handle();
  if(driveCommand) {
    /* transfer command immediately to drive controller */
    Display_ShowText(15, 15, String("Current command: " + driveCommand));
    delay(5000); // NUR ZUM TESTEN!!!
  }

  /* update outgoing data (state / batteryLevel / speed) */
  // Test_I2C
  set_state(get_state());
  set_batteryLevel(get_batteryLevel() + 2);
  set_speed(get_speed());
  delay(2000);
  // TEST_I2C END
#endif
  
  Motors_Handle();

  // read coil sensor
  sensorValue = CoilSensor_Read();
  Helpers_SerialPrintLnAndVal("Read sensor value: ", sensorValue);

  // get remote control data (motor test)
  speedVal = RemoteControl_GetThrottle();
  steeringVal = RemoteControl_GetSteering();

  // set motor speed
  Motors_ForwardAndSteering(speedVal, steeringVal);  

  // update display
  Display_Clear();
  Display_ShowText(15, 11, String("Coil L val:"));
  Display_ShowText(80, 11, String(sensorValue));
  Display_ShowText(15, 25, String("Motors speed:"));
  Display_ShowText(80, 25, String(speedVal));
  Display_ShowText(15, 39, String("Steering:"));
  Display_ShowText(80, 39, String(steeringVal));
  
  delay(50);
}