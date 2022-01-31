
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
long loopcount = 0;

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
    
    loopcount++;

    Serial.print("RunningLoop  - ");
    Serial.println(loopcount);

    CheckWiFi();    // read in any commands to clear the buffer

    CheckEncoders();

    ButtonPressed = CheckButtons();

    switch (ButtonPressed) {

        case 12:
                // Draw the menu page
        break;

        default:
            // do nothing
            // the function keys are already dealt with
        break;
    }

    delay(1000);

}