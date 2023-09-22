#include "Arduino.h"
#include "Wire.h"
#include "Servo.h"
#include "PID_v1.h"
#include "L298NX2.h"
#include "NewPing.h"
#include "Adafruit_TCS34725.h"
#include "Adafruit_SSD1306.h"
#include "lib/rgb2hue.h"
#include "lib/color.h"
#include "lib/battery.h"

#define DEBUG true

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define SCREEN_ADDRESS 0x3C

/// Function Prototypes

void autoZone();
bool outOfZone(Color::COLOR colorZone, Color::COLOR newColor);
void zoneA();
void slope();
void zoneB();
void zoneC();
void servoOpen(bool open);

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

// H-Bridge

L298NX2 motorDriver(enAPin, in1Pin, in2Pin, enBPin, in3Pin, in4Pin);

// Servo

Servo gripper;
static const uint16_t minPulse = 600;
static const uint16_t maxPulse = 2400;

static const uint8_t servoClosedPosition = 0;
static const uint8_t servoOpenPosition = 180;

// Color sensor

Adafruit_TCS34725 rgbSensor =
Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// Ultrasonic

static const uint16_t MAX_LENGTH = 300;

NewPing leftUltrasonic(leftUltrasonicPin, leftUltrasonicPin, MAX_LENGTH);
NewPing frontUltrasonic(frontUltrasonicPin, frontUltrasonicPin, MAX_LENGTH);
NewPing rightUltrasonic(rightUltrasonicPin, rightUltrasonicPin, MAX_LENGTH);

// OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/**
 * Todo list:
 * Add Encoder lib
 * Add color sensor logic
 * Add IR sensor logic
 * Program Zone A
 * Program Zone B
 * Program Zone C
 * Program slope zone
 * Program PID control for movement
 * Program PID control for pivot
 */

void setup()
{
    // Servo
    gripper.attach(gripperPin, minPulse, maxPulse);
    gripper.write(servoClosedPosition);
    
    // Color sensor
    rgbSensor.begin();
    
    // IR, these are already setup as inputs at startup
    pinMode(leftIRPin, INPUT);
    pinMode(rightIRPin, INPUT);
    
    // OLED
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void loop()
{
    
}

void autoZone()
{
    float red = 0.0f, green = 0.0f, blue = 0.0f;
    // Get color
    rgbSensor.getRGB(&red, &green, &blue);
    
    uint16_t hue = getHue(red, green, blue);
    
    Color::COLOR color = Color::getColor(red, green, blue, hue);
    
    switch (color)
    {
        case Color::COLOR_YELLOW:
            zoneA();
            break;
        case Color::COLOR_CYAN:
            slope();
            break;
        case Color::COLOR_MAGENTA:
            zoneB();
            break;
        case Color::COLOR_VIOLET:
            zoneC();
            break;
        default:
            slope();
            break;
    }
}

bool outOfZone(Color::COLOR colorZone, Color::COLOR newColor)
{
    switch (newColor)
    {
        case Color::COLOR_YELLOW:
        case Color::COLOR_CYAN:
        case Color::COLOR_MAGENTA:
        case Color::COLOR_VIOLET:
            if (newColor != colorZone)
                return true;
            break;
    }
    
    return false;
}

void zoneA()
{
    // Code for Zone A
}

void slope()
{
    
}

void zoneB()
{
    // Code for Zone B
}

void zoneC()
{
    // Code for Zone C
    
    const uint8_t motorSpeed = 100;
    // We have to do a line follower
    motorDriver.setSpeed(motorSpeed);
    motorDriver.forward();
    
    while (true)
    {
        // Are they seeing the line?
        bool leftIR = !digitalRead(leftIRPin);
        bool rightIR = !digitalRead(rightIRPin);
        
        if (leftIR && rightIR)
        {
            // Stop
            motorDriver.stop();
        }
        else if (leftIR && !rightIR)
        {
            motorDriver.stopA();
        }
        else if (!leftIR && rightIR)
        {
            motorDriver.stopB();
        }
        else
        {
            motorDriver.forward();
        }
    }
    
}

void servoOpen(bool open)
{
    gripper.write(open ? servoOpenPosition : servoClosedPosition);
}
