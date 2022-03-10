#include <Arduino.h>
#include "helpers.h"
#include "display.h"
#include "coil-sensor.h"

uint16_t sensorValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Init_Display();
  Init_CoilSensor();
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = Read_CoilSensor();
  SerialPrintLnAndVal("Read sensor value: ", sensorValue);
  ShowText_Display(String(sensorValue));
  delay(50);
}