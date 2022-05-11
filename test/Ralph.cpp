#include <Arduino.h>

// Rotary encoder pins
#define PIN_A 34
#define PIN_B 35
#define PUSH_BTN1 32
#define PIN_C 25
#define PIN_D 26
#define PUSH_BTN2 27

// A turn counter for the rotary encoder (negative = anti-clockwise)
int rotationCounter = 0;

// Flag from interrupt routine (moved=true)
volatile bool rotaryEncoder1 = false;
volatile bool rotaryEncoder2 = false;

// Interrupt routine just sets a flag when rotation is detected
void IRAM_ATTR rotary1()
{
    rotaryEncoder1 = true;
}
void IRAM_ATTR rotary2()
{
    rotaryEncoder2 = true;
}

// Rotary encoder has moved (interrupt tells us) but what happened?
// See https://www.pinteric.com/rotary.html
int8_t checkRotaryEncoder(int encoder)
{

    int8_t l = 0;
    int8_t r = 0;
    static uint8_t lrmem = 3;
    static int lrsum = 0;
    static int8_t TRANS[] = {0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0};

    // Reset the flag that brought us here (from ISR)
    //and read the pins
    if (encoder == 1)
    {
        rotaryEncoder1 = false;
        // Read BOTH pin states to deterimine validity of rotation (ie not just switch bounce)
        l = digitalRead(PIN_A);
        r = digitalRead(PIN_B);
    }
    else
    {
        rotaryEncoder2 = false;
        // Read BOTH pin states to deterimine validity of rotation (ie not just switch bounce)
        l = digitalRead(PIN_C);
        r = digitalRead(PIN_D);
    }   

    // Move previous value 2 bits to the left and add in our new values
    lrmem = ((lrmem & 0x03) << 2) + 2 * l + r;

    // Convert the bit pattern to a movement indicator (14 = impossible, ie switch bounce)
    lrsum += TRANS[lrmem];

    /* encoder not in the neutral (detent) state */
    if (lrsum % 4 != 0)
    {
        return 0;
    }

    /* encoder in the neutral state - clockwise rotation*/
    if (lrsum == 4)
    {
        lrsum = 0;
        return 1;
    }

    /* encoder in the neutral state - anti-clockwise rotation*/
    if (lrsum == -4)
    {
        lrsum = 0;
        return -1;
    }

    // An impossible rotation has been detected - ignore the movement
    lrsum = 0;
    return 0;
}

void setup()
{
    Serial.begin(115200);

    // The module already has pullup resistors on board
    pinMode(PIN_A, INPUT);
    pinMode(PIN_B, INPUT);
    pinMode(PIN_C, INPUT);
    pinMode(PIN_D, INPUT);

    // But not for the push switch
    pinMode(PUSH_BTN1, INPUT_PULLUP);
    pinMode(PUSH_BTN2, INPUT_PULLUP);

    // We need to monitor both pins, rising and falling for all states
    attachInterrupt(digitalPinToInterrupt(PIN_A), rotary1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_B), rotary1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_C), rotary2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_D), rotary2, CHANGE);
    Serial.println("Setup completed");
}

void loop()
{
    // Has rotary encoder moved?
    if (rotaryEncoder1)
    {
        // Get the movement (if valid)
        int8_t rotationValue = checkRotaryEncoder(1);

        // If valid movement, do something
        if (rotationValue != 0)
        {
            rotationCounter += rotationValue * 5;
            Serial.print(rotationValue < 1 ? "L" :  "R");
            Serial.println(rotationCounter);
        }
    }

    if (rotaryEncoder2)
    {
        // Get the movement (if valid)
        int8_t rotationValue = checkRotaryEncoder(2);

        // If valid movement, do something
        if (rotationValue != 0)
        {
            rotationCounter += rotationValue * 5;
            Serial.print(rotationValue < 1 ? "L" :  "R");
            Serial.println(rotationCounter);
        }
    }

    if (digitalRead(PUSH_BTN1) == LOW)
    {
        rotationCounter = 0;
        Serial.print("X");
        Serial.println(rotationCounter);
 
        // Wait until button released (demo only! Blocking call!)
        while (digitalRead(PUSH_BTN1) == LOW)
        {
            delay(100);
        }
    }

     if (digitalRead(PUSH_BTN2) == LOW)
    {
        rotationCounter = 0;
        Serial.print("X");
        Serial.println(rotationCounter);
 
        // Wait until button released (demo only! Blocking call!)
        while (digitalRead(PUSH_BTN2) == LOW)
        {
            delay(100);
        }
    }
}
