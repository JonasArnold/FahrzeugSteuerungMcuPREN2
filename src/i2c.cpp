#include <Arduino.h>
#include <Wire.h>                   // Bibliothek, die Funktionen für die I2C-Kommunikation enthält
#include "i2c.h"
#define SLAVE_ADDRESS 0x01          // I2C Adresse vom Slave (ESP32)

// vars
int command = 0;                    // Befehl; no_command = 0 / speed_up = 1 / speed_down = 2 / stopp = 3

void I2C_Init() {

  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);      // Funktion 'receiveDate' wird aufgerufen, wenn Daten über I2C empfangen wurden
  Wire.onRequest(sendData);         // Funktion 'sendDate' wird aufgerufen, wenn Daten über I2C angefordert wurden
}

void receiveData(int byteCount) {

  if(Wire.available()) {
    command = Wire.read();
    Wire.flush();
  }
}

void sendData() {

  byte data [] = {state, batteryLevel, speed};
  Wire.write(data, 3);
  Wire.flush();
}

int I2C_Handle() {                  // Befehl; no_command = 0 / speed_up = 1 / speed_down = 2 / stopp = 3

  if(command!=0) {
    int x = command;
    command = 0;
    return x;
  }
  return command;
}