#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

// declaration
void Init_Display();

void ShowText_Display(int16_t, int16_t, String);
void Clear_Display();

#endif