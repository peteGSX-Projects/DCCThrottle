
// Initial ESP32 test

#include <Arduino.h>
#ifndef FILESTRUCT_H 
    #include "filestruct.h"
#endif
#include "DCCThrottle.h"
#include "display.h"
#include "rotary.h"
#include "DCCWifi.h"
#include "roster.h"



int ButtonPressed = 0;


//String DefaultRosterFkeys = "Light/Sound/Brake/*Whistle/*Whistle/QS/*Coal/Blower/Blower/*Guard/*Injector/*Dcock/Aux/*Tannoy/*Wheel/*Noise/*Doors/SetOff/*Whistle/*Couple/Shunt/*Blower/*Buffer/*Whistle/*Whistle/F25/F26/*Vol -/*Vol +";
 //String RosterFKeys[2][29];
    
// bool FKeyLock[2][29];

// int MAXLOCOS = 0;
// int LocoAddress[2] = {7309,5275};
// char LocoName[2][18] = {"Jinty","42xx 2-8-0 Tank"};
// int LocoDirection[2] = {1, 1};
// int LocoSpeed[2] = {0, 0};
// int CurrentChannel = 0;
// int FunctionPressed = 0;
// bool PowerOn = true;
// bool SoundOn[2] = {false, false};
// String readString = "";
// int ADDRESS = 0; //loco address
// String LOCONAME = "";

LOCAL_ROSTER LOCALLOCOS[] = {        // Prime with defaults
  { 7309, "LMS Jinty"},
  { 5275, "42xx 2-8-0 Tank"},
  { 3433, "City of Bath"},
  { 6110, "Large Prarie"},
  { 7246, "72xx 2-8-2 Tank"} 
};


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

    MainScreenHeader();

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