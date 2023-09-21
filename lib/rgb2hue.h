/**
 * rgb2hue.h - Library used to get hue value (HSV) from RGB
 */

#ifndef RGB2HUE_H
#define RGB2HUE_H

// 255, 128, 0
unsigned char getHue(uint8_t red, uint8_t green, uint8_t blue)
{
    unsigned char maximum = max(max(red, green), blue);
    unsigned char minimum = min(min(red, green), blue);
    
    // No hue
    if (minimum == maximum) return 0;
    
    float hue = 0.0f;
    
    if (red == maximum)
    {
        hue = (green - blue) / (maximum - minimum);
    }
    else if (green == maximum)
    {
        hue = 2.0f + (red - blue) / (maximum - minimum);
    }
    else
    {
        hue = 4.0f + (red - green) / (maximum - minimum);
    }
    
    hue *= 60.0f;
    
    if (hue < 0.0f) hue += 360.0f;
    
    return static_cast<int>(hue);
}

#endif