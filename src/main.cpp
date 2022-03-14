#include <Arduino.h>
#include "helpers.h"
#include "display.h"
#include "coil-sensor.h"
#include "motors.h"

uint16_t sensorValue = 0;
uint16_t potValueAnalog, potValPct;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Init_Display();
  Init_CoilSensor();

  ShowText_Display(15, 15, String("Initializing motors..."));
  Init_Motors();

  Clear_Display();
  ShowText_Display(15, 15, String("Init done"));

  delay(2000);
}

void loop() {
  // read coil sensor
  sensorValue = Read_CoilSensor();
  SerialPrintLnAndVal("Read sensor value: ", sensorValue);

  // read poti
  potValueAnalog = analogRead(14);
  potValPct = map(potValueAnalog, 0, 4095, 0, 100);
  // set motor speed
  Forward_Motors(potValPct);
  

  // update display
  Clear_Display();
  ShowText_Display(15, 15, String("Coil L val:"));
  ShowText_Display(80, 15, String(sensorValue));
  ShowText_Display(15, 40, String("Motors pct:"));
  ShowText_Display(80, 40, String(potValPct));



  delay(50);
}