#include <Arduino.h>
#include "helpers.h"
#include "display.h"
#include "coil-sensor.h"
#include "motors.h"
#include "i2c.h"

uint16_t sensorValue = 0;
uint16_t potValueAnalog, potValPct;
uint8_t driveCommand;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Display_Init();
  CoilSensor_Init();

  Display_ShowText(15, 15, String("Initializing I2C..."));
  I2C_Init();

  Display_ShowText(15, 15, String("Initializing motors..."));
  Motors_Init();

  Display_Clear();
  Display_ShowText(15, 15, String("Init done"));

  delay(2000);

  // TEST_I2C VAR
  state = 1; batteryLevel = 3; speed = 5;
}

void loop() {

  // handle all periodic stuff
  driveCommand = I2C_Handle();
  if(driveCommand) {
    // /* transfer of command to drive controller */
    Display_ShowText(15, 15, String(driveCommand));
  }

  // Test_I2C START
  state++;
  batteryLevel++;
  speed++;
  delay(2000);
  // TEST_I2C END
  
  Motors_Handle();

  // read coil sensor
  sensorValue = CoilSensor_Read();
  Helpers_SerialPrintLnAndVal("Read sensor value: ", sensorValue);

  // read poti
  potValueAnalog = analogRead(14);
  potValPct = map(potValueAnalog, 0, 4095, 0, 100);
  // set motor speed
  Motors_Forward(potValPct);
  

  // update display
  Display_Clear();
  Display_ShowText(15, 15, String("Coil L val:"));
  Display_ShowText(80, 15, String(sensorValue));
  Display_ShowText(15, 40, String("Motors pct:"));
  Display_ShowText(80, 40, String(potValPct));



  delay(50);
}