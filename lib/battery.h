/**
 * battery.h - Library used to calculate the voltage of the battery.
 */

#ifndef BATTERY_H
#define BATTERY_H

#include "Arduino.h"
#include <stdint.h>

float getBatteryLevel(uint8_t batteryPin)
{
    uint16_t raw = analogRead(batteryPin);
    
    float voltage = (raw * 5.0f / 1023.0f) * 2.0f;
    
    return voltage;
}

bool isBatteryLevelGood(uint8_t batteryPin)
{
    float voltage = getBatteryLevel(batteryPin);
    float cutoff_voltage = 6.0f;
    
    return voltage > cutoff_voltage;
}

bool isBatteryLevelGreat(uint8_t batteryPin)
{
    float voltage = getBatteryLevel(batteryPin);
    float warning_voltage = 7.8f;
    
    return voltage > warning_voltage;
}

#endif
