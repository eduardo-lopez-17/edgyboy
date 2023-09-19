#include "Arduino.h"
#include "Servo.h"
#include "PID_v1.h"
#include "L298NX2.h"
#include "NewPing.h"

/// Function Prototypes

void zoneA();
void zoneB();
void zoneC();

/// Variables

static const uint8_t in1Pin = 4;
static const uint8_t in2Pin = 12;
static const uint8_t in3Pin = 6;
static const uint8_t in4Pin = 7;

static const uint8_t enAPin = 5;
static const uint8_t enBPin = 11;

static const uint8_t leftEncoder = 2;
static const uint8_t rightEncoder = 3;

static const uint8_t gripperPin = 10;

static const uint8_t leftIRPin = 8;
static const uint8_t rightIRPin = 9;

static const uint8_t leftUltrasonicPin = A0;
static const uint8_t frontUltrasonicPin = A1;
static const uint8_t rightUltrasonicPin = A2;

static const uint8_t batteryPin = A3;

Servo gripper;
static const uint16_t minPulse = 600;
static const uint16_t maxPulse = 2400;

/**
 * Todo list:
 * Add Encoder lib
 * Add servo logic
 * Add color sensor logic
 * Add IR sensor lib
 * Add IR sensor logic
 * Auto-zone Detect zones automatically
 * Program Zone A
 * Program Zone B
 * Program Zone C
 * Program slope zone
 * Program PID control for movement
 * Program PID control for pivot
 */

void setup()
{
    gripper.attach(gripperPin, minPulse, maxPulse);
    gripper.write(0);
}

void loop()
{
    
}

void zoneA()
{
    // Code for Zone A
}

void zoneB()
{
    // Code for Zone B
}

void zoneC()
{
    // Code for Zone C
}