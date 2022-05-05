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
void Display_UpdateNewValues(int state, int batPct, bool connected, int valTL, int valTR, int valML, int valMR, int valBL, int valBR);

/* Show a text on the display, x, y, text */
void Display_ShowText(int16_t, int16_t, String);

/* clear complete content of display */
void Display_Clear();

#endif