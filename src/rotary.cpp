#include <Arduino.h>

#ifndef FILESTRUCT_H 
#include "filestruct.h"
#endif
#include "rotary.h"
#include "display.h"
#include "DCCWifi.h"

// Rotary encoder pins
#define PIN_A 34
#define PIN_B 35
#define PUSH_BTN1 32
#define PIN_C 25
#define PIN_D 26
#define PUSH_BTN2 27

// A turn counter for the rotary encoder (negative = anti-clockwise)
int rotationCounter1 = 0;
int rotationCounter2 = 0;

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

void StartEncoders()
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

void CheckEncoders()
{
    // Has rotary encoder moved?
    if (rotaryEncoder1 == true)
    {
        // Get the movement (if valid)
        int8_t rotationValue = checkRotaryEncoder(1);

        // If valid movement, do something
        if (rotationValue != 0)
        {
            rotationCounter1 += rotationValue;
            Serial.print(rotationValue < 1 ? "L" :  "R");
            Serial.println(rotationCounter1);
            if (rotationCounter1 < 0){
                rotationCounter1 = 0;
            }
            if (rotationCounter1 > 127) {
                rotationCounter1 = 127;
            }
            Serial.print("Channel 0 Value - ");
            Serial.println(rotationCounter1 * 5);

            LocoSpeed[0] = rotationCounter1 * 5; // go up in fives else its too slow.
            ShowSpeed(0);
            DoDCCThrottle(0);

        }
        rotaryEncoder1 = false;

    }

    if (rotaryEncoder2 ==  true)
    {
        // Get the movement (if valid)
        int8_t rotationValue = checkRotaryEncoder(2);

        // If valid movement, do something
        if (rotationValue != 0)
        {
            rotationCounter2 += rotationValue;
            Serial.print(rotationValue < 1 ? "L" :  "R");
            Serial.println(rotationCounter2);
             if (rotationCounter2 < 0){
                rotationCounter2 = 0;
            }
            if (rotationCounter2 > 127) {
                rotationCounter2 = 127;
            }
            Serial.print("Channel 1 Value - ");
            Serial.println(rotationCounter2 * 5);

            LocoSpeed[1] = rotationCounter2 * 5;
            ShowSpeed(1);
            DoDCCThrottle(1);

        }

        rotaryEncoder2 = false;
    }

    if (digitalRead(PUSH_BTN1) == LOW)
    {
        //rotationCounter1 = 0;
        Serial.print("X");
        Serial.println(rotationCounter1);

        // Toggle the direction
        if (LocoDirection[0] == 1)
            {
                LocoDirection[0] = 0;
            }
            else
            {
                LocoDirection[0] = 1;
            }
 
        // Wait until button released (demo only! Blocking call!)
        while (digitalRead(PUSH_BTN1) == LOW)
        {
            delay(100);
        }
        ShowSpeed(0);
    }

     if (digitalRead(PUSH_BTN2) == LOW)
    {
        //rotationCounter2 = 0;
        Serial.print("X");
        Serial.println(rotationCounter2);
 
        // Toggle the direction
        if (LocoDirection[1] == 1) 
            {
                LocoDirection[1] = 0;
            }
            else
            {
                LocoDirection[1] = 1;
            } 
            
          // Wait until button released (demo only! Blocking call!)
        while (digitalRead(PUSH_BTN2) == LOW)
        {
            delay(100);
        }

        ShowSpeed(1);
    }
}
