#include "helpers.h"

void Helpers_SerialPrintLnAndVal(String text, uint16_t value)
{
    Serial.print(text);
    Serial.print(value);
    Serial.print("\n");
}