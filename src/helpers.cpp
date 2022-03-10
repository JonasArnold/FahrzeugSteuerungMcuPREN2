#include "helpers.h"

void SerialPrintLnAndVal(String text, uint16_t value)
{
    Serial.print(text);
    Serial.print(value);
    Serial.print("\n");
}