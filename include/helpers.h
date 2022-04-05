#ifndef HELPERS_H_
#define HELPERS_H_

#include <Arduino.h>

/* prints a description + value line to the serial monitor */
void Helpers_SerialPrintLnAndVal(String, uint16_t);

/* prints a description + value line for two values to the serial monitor */
void Helpers_SerialPrintLnAndTwoVals(String text, uint16_t value, String text2, uint16_t value2)

#endif