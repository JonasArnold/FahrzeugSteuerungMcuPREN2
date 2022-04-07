#ifndef START_BUTTON_H_
#define START_BUTTON_H_

#include <Arduino.h>

/* initialization */
void StartButton_Init(void);
/* de-initialization */
void StartButton_Deinit(void);


/* gets the state of the button. 
   returns: pressed = true, not pressed = false */
bool StartButton_GetState(void);

#endif