#include <Wire.h>                   // Bibliothek, die Funktionen für die I2C-Kommunikation enthält
#include <Arduino.h>
#include "i2c.h"
#include <Adafruit_I2CDevice.h>

//#define SLAVE_ADDRESS 0x01          // I2C Adresse vom Slave (ESP32)
#define I2C_SDA 25
#define I2C_SCD 26

// vars
int command = 0;                    // Befehl; no_command = 0 / speed_up = 1 / speed_down = 2 / stopp = 3
byte state;
byte batteryLevel;
byte speed;

//Adafruit_I2CDevice i2c_dev = Adafruit_I2CDevice(I2C_ADDRESS);
TwoWire comWire = TwoWire(0);

void I2C_Init() {

  Serial.begin(9600);
  comWire.begin(I2C_SDA, I2C_SCD);
  comWire.onReceive(receiveData);      // Funktion 'receiveDate' wird aufgerufen, wenn Daten über I2C empfangen wurden
  comWire.onRequest(sendData);         // Funktion 'sendDate' wird aufgerufen, wenn Daten über I2C angefordert wurden
}

void receiveData(int byteCount) {

  if(comWire.available()) {
    command = comWire.read();
    comWire.flush();
  }
}

void sendData() {

  byte data [] = {state, batteryLevel, speed};
  comWire.write(data, 3);
  comWire.flush();
}

int I2C_Handle() {                  // Befehl; no_command = 0 / speed_up = 1 / speed_down = 2 / stopp = 3

  if(command!=0) {
    int x = command;
    command = 0;
    return x;
  }
  return command;
}

int get_state() {
  return state;
}
void set_state(int x) {
  state = x;
}

int get_batteryLevel() {
  return batteryLevel;
}
void set_batteryLevel(int x) {
  batteryLevel = x;
}

int get_speed() {
  return speed;
}
void set_speed(int x) {
  speed = x;
}