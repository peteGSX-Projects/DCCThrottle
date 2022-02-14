
// Initial ESP32 test

#include <Arduino.h>

//#include "filestruct.h"

#include "DCCThrottle.h"
#include "display.h"
#include "rotary.h"
#include "DCCWifi.h"
#include "roster.h"


int ButtonPressed = 0;
bool LocoChosen = false;
bool RosterFound = false;
long loopcount = 0;

void WaitForIt(char C)
{
  while (true)
  {
    if (Serial.available() > 0)
      if (Serial.read() == C)
        break;
  }
}

void setup()
{
    Serial.begin(115200);


    TFTSetup();             // initialise the TFT
    Serial.println("TFT Setup Run");

    StartEncoders();        // initialise the rotary encoderes
    Serial.println("Encoders Started");
    
    WiFiSetup();            // Setup the WiFi connection


    Serial.println("Sending Roster Command");
    screenmessage("Getting Roster from CS");
    SetupRoster();          // Get the roster data from the server

    Serial.println("RosterProcessed");    //Serial.println("Getting FKeys for Channel 0");

    //Serial.println("Processing Default Function Keys");
    GetFunctionKeys();      // Get the functions for loco on channel 0


    MainScreen();       // Display the initial screen

    WaitForIt('A');

    RosterScreen();

    WaitForIt('B');

    KeypadScreen();

    WaitForIt('C');

    MainScreen();       // Display the initial screen

    Serial.println("Setup completed");
}

void loop()
{
    
    loopcount++;

    // Serial.print("RunningLoop  - ");
    // Serial.println(loopcount);

    //CheckWiFi();    // read in any commands to clear the buffer

    CheckEncoders();

    // Serial.print("Current Screen : ");
    // Serial.println(CURRENTSCREENPAGE);

    switch (CURRENTSCREENPAGE) {

        case 1:

        ButtonPressed = CheckButtons();

        //Serial.print("Button State : ");
        //Serial.println(ButtonPressed);

            switch (ButtonPressed) {

                case 12:        // Channel 0 Select
                    Serial.println("Selecting loco channel 0");
                    CurrentChannel = 0;
                    DrawRosterList();
                break;  

                case 14:        // Channel 1 Select
                     Serial.println("Selecting loco channel 1");
                     CurrentChannel = 1;
                     DrawRosterList();
                break;

                default:
                    // do nothing
                    // the function keys are already dealt with
                break;
            }

        break;

        case 2:

            ButtonPressed = CheckRosterButtons();

            if (ButtonPressed >= 0) {
                Serial.print("Roster Item Selected = ");
                Serial.println(ButtonPressed);

            }

        break;

        case 3:

            LocoChosen = CheckNumberPad();
            if (LocoChosen == true){
                LocoAddress[CurrentChannel] = ADDRESS;
                //LocoName[CurrentChannel] = "";
                sprintf(LocoName[CurrentChannel] , "%s", "Unassigned");
                // perhaps check that this is not already in the roster.
                //
            }

        break;

    }

    //delay(200);

}