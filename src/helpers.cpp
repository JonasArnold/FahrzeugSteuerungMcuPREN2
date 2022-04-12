#include "helpers.h"
#include "configuration.h"

void Helpers_SerialPrintLnAndVal(String text, uint16_t value)
{
#ifdef DEBUGGING_ENABLED
    Serial.print(text);
    Serial.print(value);
    Serial.print("\n");
#endif
}

void Helpers_SerialPrintLnAndTwoVals(String text, uint16_t value, String text2, uint16_t value2)
{
#ifdef DEBUGGING_ENABLED
    Serial.print(text);
    Serial.print(value);
    Serial.print(text2);
    Serial.print(value2);
    Serial.print("\n");
#endif
}