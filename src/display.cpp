#include "display.h"
#include <SPI.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, 5, 4);

void Display_Init()
{
  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  Serial.println("Display initialized.");
}

void Display_SetupBase()
{
    display.drawHorizontalLine(0, 13, 128);
    display.drawHorizontalLine(0, 27, 128);
    display.drawHorizontalLine(0, 41, 128);
}

void Display_ShowText(int16_t posX, int16_t posY, String text)
{
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(posX, posY, text);
  display.setFont(ArialMT_Plain_10);

  display.display();
}

void Display_Clear()
{
  display.clear();
}