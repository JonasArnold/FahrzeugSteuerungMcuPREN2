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
void ShowText_Display(int16_t posX, int16_t posY, String text)
{
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(posX, posY, text);
  display.setFont(ArialMT_Plain_10);

  display.display();
}

void Clear_Display()
{
  display.clear();
}