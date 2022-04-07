#ifndef START_BUTTON_H_
#define START_BUTTON_H_

#include <Arduino.h>

/* initialization */
void StartButton_Init(void);
/* de-initialization */
void StartButton_Deinit(void);
/* handle internal stuff, function to call in super loop (peridocally) */
int StartButton_Handle(void);

#endif