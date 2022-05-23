#include <Arduino.h>
#include <Wire.h>
#include "i2c.h"
#include "display.h"
#include "configuration.h"

#define SDA_PIN 25
#define SCL_PIN 26
#define I2C_SLAVE_ADDR 0x40           // I2C Adresse vom Slave (ESP32)

// vars
static Command command = Command::None;
byte state = 0, batteryLevel = 0, speed = 0;  // variables that will be send to master

// function declaration
static void receiveData(int byteCount);
static void sendData(void);

void I2C_Init() {
#ifdef COMMUNICATION_ENABLED
    bool res = Wire1.begin((uint8_t)I2C_SLAVE_ADDR, SDA_PIN, SCL_PIN, 400000);
    if (!res) {
#ifdef DEBUGGING_ENABLED
        Serial.println("I2C slave init failed");
#endif
        Display_Clear();
        Display_ShowText(15, 15, String("I2C slave init failed"));
        while(res == false) {
            delay(1000);
            res = Wire1.begin((uint8_t)I2C_SLAVE_ADDR, SDA_PIN, SCL_PIN, 400000);
        }
    }

    Wire1.onReceive(receiveData);
    Wire1.onRequest(sendData);

#ifdef DEBUGGING_ENABLED
    Serial.println("I2C slave init done");
#endif
#endif
}

Command I2C_Handle()
{                    
    if(command != Command::None) {
        Command command_out = command;
        command = Command::None;
        return command_out;
    }
    return Command::None;
}

// function that runs whenever the master sends an empty packet.
// this function is registered as an event, see setup().
// do not perform time-consuming tasks inside this function,
// do them elsewhere and simply read the data you wish to
// send inside here.
static void sendData(void) {
    byte data [] = {state, batteryLevel, speed};
    Wire1.write(data, 3);
    Serial.print("Data sent: [");
    for (uint8_t i = 0; i < 3; i++)
    {
        Serial.print(data[i]);
        if(i==2) { Serial.println("]"); }
        else     { Serial.print(", "); }
    }
}

// function that executes whenever a complete and valid packet
// is received from master
// this function is registered as an event, see setup()
static void receiveData(int howMany) {

    while(Wire1.available()) {
        int recieved = Wire1.read();
        Serial.print("Data received: ");
        Serial.println(recieved);
        if      (recieved == 0) { command = Command::None; }
        else if (recieved == 1) { command = Command::SpeedDown; }
        else if (recieved == 2) { command = Command::SpeedUp; }
        else if (recieved == 3) { command = Command::Stop; }
        else { 
            Serial.println("Unknown I2C command arrived");
        }
    }
}

// getters & setters
void I2C_SetDeviceState(byte pState) {

    state = pState;
}
void I2C_SetBatteryLevel(byte pBatteryLevel) {

    batteryLevel = pBatteryLevel;
}
void I2C_SetSetSpeed(byte pSetSpeed) {
    speed = pSetSpeed;
}