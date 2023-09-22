/**
 * color.h - Library used to clasificate colors, also includes definitions.
 */

#ifndef COLOR_H
#define COLOR_H

#include "stdint.h"
namespace Color
{
    enum COLOR
    {
        COLOR_NO_COLOR = 0b0000,
        COLOR_UNKNOWN_COLOR = 0b0001,
        COLOR_RED = 0b0010,
        COLOR_ROSE = 0b0011,
        COLOR_MAGENTA = 0b0100,
        COLOR_VIOLET = 0b0101,
        COLOR_BLUE = 0b0110,
        COLOR_AZURE = 0b0111,
        COLOR_CYAN = 0b1000,
        COLOR_SPRING_GREEN = 0b1001,
        COLOR_GREEN = 0b1010,
        COLOR_CHARTREUSE_GREEN = 0b1011,
        COLOR_YELLOW = 0b1100,
        COLOR_ORANGE = 0b1101,
        COLOR_WHITE = 0b1110,
        COLOR_BLACK = 0b1111
    };

    COLOR get_color(uint8_t red, uint8_t green, uint8_t blue, uint16_t hue)
    {
        const uint8_t white_cutoff = 64;
        const uint8_t black_cutoff = 255 - 64;
        
        if (red <= white_cutoff && green <= white_cutoff && blue <= white_cutoff)
        {
            return WHITE;
        }
        else if (red >= black_cutoff && green >= black_cutoff && blue >= black_cutoff)
        {
            return BLACK;
        }
        
        // Check which color is
        if (hue < 15)
        {
            return COLOR_RED;
        }
        else if (hue < 45)
        {
            return COLOR_ORANGE;
        }
        else if (hue < 75)
        {
            return COLOR_YELLOW;
        }
        else if (hue < 105)
        {
            return COLOR_CHARTREUSE_GREEN;
        }
        else if (hue < 135)
        {
            return COLOR_GREEN;
        }
        else if (hue < 165)
        {
            return COLOR_SPRING_GREEN;
        }
        else if (hue < 195)
        {
            return COLOR_CYAN;
        }
        else if (hue < 225)
        {
            return COLOR_AZURE;
        }
        else if (hue < 255)
        {
            return COLOR_BLUE;
        }
        else if (hue < 285)
        {
            return COLOR_VIOLET;
        }
        else if (hue < 315)
        {
            return COLOR_MAGENTA;
        }
        else if (hue < 345)
        {
            return COLOR_ROSE;
        }
        else
        {
            return COLOR_RED;
        }
    }
}

#endif