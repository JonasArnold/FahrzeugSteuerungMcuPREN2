#include <Arduino.h>
#include <Wire.h>                   // Bibliothek, die Funktionen für die I2C-Kommunikation enthält
#include <WireSlave.h>
#include "i2c.h"

//#define SLAVE_ADDRESS 0x01          // I2C Adresse vom Slave (ESP32)
#define I2C_SDA 25
#define I2C_SCD 26
#define I2C_SLAVE_ADDR 0x04

// vars
int command = 0;                    // Befehl; no_command = 0 / speed_up = 1 / speed_down = 2 / stopp = 3
byte state;
byte batteryLevel;
byte speed;

void requestEvent();
void receiveEvent(int howMany);

void I2C_Init() {

  bool res = WireSlave.begin(I2C_SDA, I2C_SCD, I2C_SLAVE_ADDR);
  if (!res) {
      Serial.println("I2C slave init failed");
      while(1) delay(100);
  }

  WireSlave.onRequest(requestEvent);
  WireSlave.onReceive(receiveEvent);

}

// function that runs whenever the master sends an empty packet.
// this function is registered as an event, see setup().
// do not perform time-consuming tasks inside this function,
// do them elsewhere and simply read the data you wish to
// send inside here.
void requestEvent()
{
    static byte y = 0;

    WireSlave.print("y is ");
    WireSlave.write(y++);
}


// function that executes whenever a complete and valid packet
// is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
    while (1 < WireSlave.available()) // loop through all but the last byte
    {
        char c = WireSlave.read();  // receive byte as a character
        Serial.print(c);            // print the character
    }

    int x = WireSlave.read();   // receive byte as an integer
    Serial.println(x);          // print the integer
}

/*
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
*/

int I2C_Handle() {                  // Befehl; no_command = 0 / speed_up = 1 / speed_down = 2 / stopp = 3

  WireSlave.update();

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