#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <Arduino.h>

/* initialization */
void Display_Init();


/* Setup the base structure of the display */
void Display_SetupBase();

/* Show a text on the display, x, y, text */
void Display_ShowText(int16_t, int16_t, String);
/* clear complete content of display */
void Display_Clear();

#endif