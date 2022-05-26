#ifndef MY_WIFI_H_
#define MY_WIFI_H_

#include <Arduino.h>

/* initialization */
void MyWiFi_Init();

/*  Connect to the wifi
    give ssid and passphrase as char array, 
    timeout in seconds
*/
bool MyWiFi_Connect(const char *ssid, const char *passphrase, int timeout);

/*  Disconnects from the wifi 
*/
void MyWiFi_Disconnect();

/*  Returns the local IPv4 Address of the device
*/
IPAddress MyWiFi_GetIPv4();

/*  Returns the local IPv4 Address of the device as string
*/
String MyWiFi_GetIPv4String();


#endif