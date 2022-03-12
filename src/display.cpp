#include "display.h"


// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, 5, 4);

// init
void Init_Display()
{
  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  Serial.println("Display initialized.");
}

// show text
void ShowText_Display(String text)
{
  display.clear();

  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(15, 15, text);
  display.setFont(ArialMT_Plain_16);

  display.display();
}