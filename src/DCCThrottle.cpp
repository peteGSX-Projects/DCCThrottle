
// Initial ESP32 test

#include <Arduino.h>

//#include "filestruct.h"

#include "DCCThrottle.h"
#include "display.h"
#include "rotary.h"
#include "DCCWifi.h"
#include "roster.h"


int ButtonPressed = 0;
bool RosterFound = false;

void setup()
{
    Serial.begin(115200);

    TFTSetup();             // initialise the TFT
    StartEncoders();        // initialise the rotary encoderes
    WiFiSetup();            // Setup the WiFi connection


    Serial.println("Sending Roster Command");
    SetupRoster();          // Get the roster data from the server

    Serial.println("RosterProcessed");    //Serial.println("Getting FKeys for Channel 0");

    //Serial.println("Processing Default Function Keys");
    GetFunctionKeys();      // Get the functions for loco on channel 0

    MainScreen();

    Serial.println("Setup completed");
}

void loop()
{
    
    CheckEncoders();

    ButtonPressed = CheckButtons();

    switch (ButtonPressed) {

        case 12:
                // Draw the next page
        break;

        case 13:
                // Draw the Menu
        break;

        default:
            // do nothing
        break;
    }

    

}