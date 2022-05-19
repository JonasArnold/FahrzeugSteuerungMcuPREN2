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

long Helpers_LimitedMap(long x, long in_min, long in_max, long out_min, long out_max) {
    // limit values
    if(x > in_max) { x = in_max; }
    else if(x < in_min) { x = in_min; };
    
    const long run = in_max - in_min;
    if(run == 0){
        log_e("map(): Invalid input range, min == max");
        return -1; // AVR returns -1, SAM returns 0
    }
    const long rise = out_max - out_min;
    const long delta = x - in_min;
    return (delta * rise) / run + out_min;
}