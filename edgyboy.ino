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
//#include "lib/battery.h"

#define DEBUG true

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define SCREEN_ADDRESS 0x3C

/// Function Prototypes

void autoZone();
bool outOfZone(Color::COLOR colorZone, Color::COLOR newColor);
Color::COLOR autoClasifyColorZone(Color::COLOR color);
void calibrateColor(float &red, float &green, float &blue);
void zoneA();
void slope();
void zoneB();
void zoneC();
void runHalfSquare();
void turnLeft();
void turnRight();
void servoOpen(bool open);
Color::COLOR calculateColor();

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

//static const uint8_t leftIRPin = 8;
//static const uint8_t rightIRPin = 9;

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
Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_60X);

static const uint8_t redMin = 95;
static const uint8_t redMax = 126;
static const uint8_t greenMin = 93;
static const uint8_t greenMax = 143;
static const uint8_t blueMin = 63;
static const uint8_t blueMax = 120;

// Ultrasonic

static const uint16_t MAX_LENGTH = 300;

NewPing leftUltrasonic(leftUltrasonicPin, leftUltrasonicPin, MAX_LENGTH);
NewPing frontUltrasonic(frontUltrasonicPin, frontUltrasonicPin, MAX_LENGTH);
NewPing rightUltrasonic(rightUltrasonicPin, rightUltrasonicPin, MAX_LENGTH);

// OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/**
 * Todo list:
 * Define what to do if battery is low or a little low
 * Add Encoder lib
 * Add color sensor logic
 * Add IR sensor logic
 * Program Zone A
 * Program Zone B
 * Program Zone C (Work in progress)
 * Program slope zone
 * Program PID control for movement
 * Program PID control for pivot
 */

void setup()
{
    // Servo
    gripper.attach(gripperPin, minPulse, maxPulse);
    gripper.write(servoOpenPosition);
    
    // Color sensor
    rgbSensor.begin();
    // Turn on LED
    rgbSensor.setInterrupt(false);
    
    // OLED
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();
}

void loop()
{
    autoZone();
}

void autoZone()
{
    Color::COLOR color = calculateColor();
    
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

Color::COLOR autoClasifyColorZone(Color::COLOR color)
{
    
}

void calibrateColor(float &red, float &green, float &blue)
{
    red = constrain(map(red, redMin, redMax, 0, 255), 0, 255);
    green = constrain(map(green, greenMin, greenMax, 0, 255), 0, 255);
    blue = constrain(map(blue, blueMin, blueMax, 0, 255), 0, 255);
}

void zoneA()
{
    // Code for Zone A
    
    
    for (;;)
    {
        // Este codigo usa el metodo de la mano izquierda para resolver el laberinto
    
        const uint8_t distanceForNextSquare = 15;
        
        bool canGoLeft = leftUltrasonic.ping_cm() < distanceForNextSquare;
        bool canGoFront = frontUltrasonic.ping_cm() < distanceForNextSquare;
        bool canGoRight = rightUltrasonic.ping_cm() < distanceForNextSquare;
        
        if (canGoLeft)
        {
            // Esta libre la zona izquierda, podemos seguir a la izquierda
            turnLeft();
        }
        else if (canGoRight)
        {
            // Esta libre la zona derecha, podemos seguir a la derecha
            turnRight();
        }
        else if (!canGoFront)
        {
            // No hay de otra, regresa de donde veniste
            turnLeft();
            turnLeft();
        }
        
        runHalfSquare();
        
        Color::COLOR color = calculateColor();
        
        if (color == Color::COLOR_BLACK)
        {
            turnLeft();
            turnLeft();
        }
        
        runHalfSquare();
        
        if (outOfZone(Color::COLOR_YELLOW, color))
        {
            // Look at the front
            turnLeft();
            break;
        }
    }
    
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
        
        // if (leftIR && rightIR)
        // {
        //     // Stop
        //     motorDriver.stop();
        // }
        // else if (leftIR && !rightIR)
        // {
        //     motorDriver.stopA();
        // }
        // else if (!leftIR && rightIR)
        // {
        //     motorDriver.stopB();
        // }
        // else
        // {
        //     motorDriver.forward();
        // }
    }
    
}

void runHalfSquare()
{
    motorDriver.forward();
    
    const uint16_t timeToWait = 100;
    delay(timeToWait);
    
    motorDriver.stop();
    
    const uint16_t estabilize = 10;
    delay(estabilize);
}

void turnLeft()
{
    motorDriver.backwardA();
    motorDriver.forwardB();
    
    const uint16_t timeToWait = 100;
    delay(timeToWait);
    
    motorDriver.stop();
    
    const uint16_t estabilize = 10;
    delay(estabilize);
}

void turnRight()
{
    motorDriver.forwardA();
    motorDriver.backwardB();
    
    const uint16_t timeToWait = 500;
    delay(timeToWait);
    
    motorDriver.stop();
    
    const uint16_t estabilize = 10;
    delay(estabilize);
}

void servoOpen(bool open)
{
    gripper.write(open ? servoOpenPosition : servoClosedPosition);
}

Color::COLOR calculateColor()
{
    float red = 0.0f, green = 0.0f, blue = 0.0f;
    // Get color
    rgbSensor.getRGB(&red, &green, &blue);
    
    uint16_t hue = getHue(red, green, blue);
    
    return Color::getColor(red, green, blue, hue);
}
