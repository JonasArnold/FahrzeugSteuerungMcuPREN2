#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <Arduino.h>

/* initialization */
void Display_Init();


/* shows a text on the display and the logo */
void Display_ShowInitText(String text);

/* Setup the base structure of the display */
void Display_SetupBase();

/* Update the values displayed on the base structure of the display. */
void Display_UpdateNewValues(String state, int batPct, bool connected, int coilVal, int speedval, int steerVal, int rpmL, int rpmR);

/* Show a text on the display, x, y, text */
void Display_ShowText(int16_t, int16_t, String);

/* clear complete content of display */
void Display_Clear();

#endif