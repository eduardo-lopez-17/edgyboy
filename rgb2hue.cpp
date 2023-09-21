#include "rgb2hue.h"
#include <stdint.h>
#include <stdlib.h>

void getHue(uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t maximum = max(red, green);
    maximum = max(maximum, blue);
}
