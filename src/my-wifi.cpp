#ifdef ESP32
#include <WiFi.h>
#include <AsyncUDP.h>
#elif defined (ESP8266)
#include <ESPAsyncUDP.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#else
#error Platform not supported
#endif

#define DEBUGGING_ENABLED

/*  Following libraries shall be included:
 *  ESP32: 
 *  ESP Async WebServer
 *  ESP8266:
 *  ??
 */

#include <Arduino.h>
#include "my-wifi.h"

void MyWiFi_Init(){
    // Make sure you're in station mode    
    WiFi.mode(WIFI_STA);
}


bool MyWiFi_Connect(const char *ssid, const char *passphrase, int timeout){
#ifdef DEBUGGING_ENABLED
    Serial.println("");
    Serial.print("Connecting to WiFi with SSID ");
    Serial.print(ssid);
#endif

    if (passphrase != NULL){
        WiFi.begin(ssid, passphrase);
    } else {
        WiFi.begin(ssid);
    }

    int counter = 0;
    bool connected = (WiFi.status() == WL_CONNECTED);

    // while not connected and timeout not reached
    while (connected == false && counter <= timeout) {
        counter++;              // increase counter
        delay(1000);            // wait
        connected = (WiFi.status() == WL_CONNECTED); // update state
#ifdef DEBUGGING_ENABLED
        Serial.print(".");
#endif
    }

    // timeout reached or connected
#ifdef DEBUGGING_ENABLED
    Serial.println("");
    if(connected) {
        Serial.print(F("Connected to WiFi with IP: "));
        Serial.println(WiFi.localIP());
    } else {
        Serial.print("Connection to WiFi failed.");
    }
#endif
    
    return connected;
}

void MyWiFi_Disconnect(){
    WiFi.disconnect();
}

IPAddress MyWiFi_GetIPv4(){
    return WiFi.localIP();
}

String MyWiFi_GetIPv4String(){
    IPAddress ip = WiFi.localIP();
    return ip.toString();
}