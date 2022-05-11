
// Initial ESP32 test

#include <Arduino.h>

#include "filestruct.h"

#include "main.h"
#include "display.h"
#include "rotary.h"
#include "DCCWifi.h"
#include "roster.h"

// struct LOCO_ROSTER LOCOS[20];
// struct LOCAL_ROSTER LOCALLOCOS[20] = {     // Prime with defaults in case no roster found
//   { 7309, "LMS Jinty"},
//   { 5275, "42xx 2-8-0 Tank"},
//   { 3433, "City of Bath"},
//   { 6110, "Large Prarie"},
//   { 7246, "72xx 2-8-2 Tank"} 
// };


int ButtonPressed = 0;
bool RosterFound = false;




void setup()
{
    Serial.begin(115200);

    TFTSetup();             // initialise the TFT
    StartEncoders();        // initialise the rotary encoderes
    WiFiSetup();            // Setup the WiFi connection


    Serial.println("Sending Roster Command");
    GetRoster();          // Get the roster data from the server

    //Serial.println("Getting FKeys for Channel 0");

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