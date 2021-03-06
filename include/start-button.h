#ifndef START_BUTTON_H_
#define START_BUTTON_H_

#include <Arduino.h>

/* initialization */
void StartButton_Init(void);
/* de-initialization */
void StartButton_Deinit(void);


/* returns if a button event happened
   after calling this method, a potential button event is reset
   example: Function returns true because button was pressed, next time the function is called it returns false again*/
bool StartButton_GetEventHappened(void);

/* gets the state of the button. 
   returns: pressed = true, not pressed = false 
*/
bool StartButton_GetState(void);

#endif