#include <Arduino.h>
#include <Wire.h>
#include "i2c.h"

#define SDA_PIN 25
#define SCL_PIN 26
#define I2C_SLAVE_ADDR 0x40           // I2C Adresse vom Slave (ESP32)

// vars
int command = 0;                      // Befehl; no_command = 0 / speed_up = 1 / speed_down = 2 / stopp = 3
byte state;
byte batteryLevel;
byte speed;

// function declaration
static void receiveData(int byteCount);
static void sendData(void);

void I2C_Init() {
    Wire.setPins(SDA_PIN, SCL_PIN);
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
    bool res = Wire.begin((uint8_t)I2C_SLAVE_ADDR);
    if (!res) {
        Serial.println("I2C slave init failed");
        while(1) delay(100);
    }

    Serial.println("I2C slave init done");
}

int I2C_Handle() {                    // Befehl; no_command = 0 / speed_up = 1 / speed_down = 2 / stopp = 3

    if(command!=0) {
        int command_out = command;
        command = 0;
        return command_out;
    }
    return command;
}

// function that runs whenever the master sends an empty packet.
// this function is registered as an event, see setup().
// do not perform time-consuming tasks inside this function,
// do them elsewhere and simply read the data you wish to
// send inside here.
static void sendData() {

    byte data [] = {state, batteryLevel, speed};
    Wire.write(data, 3);
}

// function that executes whenever a complete and valid packet
// is received from master
// this function is registered as an event, see setup()
static void receiveData(int howMany) {

    if(Wire.available()) {
        command = Wire.read();
        Wire.flush();
    }
}

// getters & setters
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