
// Initial ESP32 test

#include <Arduino.h>

//#include "filestruct.h"

#include "DCCThrottle.h"
#include "display.h"
#include "rotary.h"
#include "DCCWifi.h"
#include "roster.h"


int ButtonPressed = 0;
int KeypadButton = 0;
bool LocoChosen = false;
bool RosterFound = false;
long loopcount = 0;

//WiFi Server stuff and WiFi logon Credentials
//const char *SSID = "BTHub6-Q9JX";
//const char *PASSWORD = "mEtdu6dt3ivN";
// const char *SSID = "DCCEX_c25024";
// const char *PASSWORD = "Monarch1466";
//const char *host[] = "192.168.4.1";
//const int *port = 2560;

//OTA stuff included here
// String projectTitle = "DCC++EX WiFi Throttle";
// String projectDesc =
//     "ESP32 for YouTube demonstration! <p>You can write text here or <b>html</b>.</p> \
//   <p>Note: Try this on another ESP32 device before demo. And other important info. Keep it concise!</p>";

// #include "Identification.h"


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

    // Everything for OTA updates
    //otaInit();

    TFTSetup();             // initialise the TFT
    Serial.println("TFT Setup Run");

    StartEncoders();        // initialise the rotary encoderes
    Serial.println("Encoders Started");
    
    WiFiSetup();            // Setup the WiFi connection to CS


    Serial.println("Sending Roster Command");
    screenmessage("Getting Roster from CS");
    SetupRoster();          // Get the roster data from the server

    Serial.println("RosterProcessed");    //Serial.println("Getting FKeys for Channel 0");

    MainScreen();       // Display the initial screen

    Serial.println("Setup completed");
}

void loop()
{
    
    loopcount++;

    CheckWiFi();    // read in any commands to clear the buffer


    switch (CURRENTSCREENPAGE) {

        case 1:

        CheckEncoders();  // These are only active on page 1 - Main Screen

        ButtonPressed = CheckButtons();

            switch (ButtonPressed) {


                case 12:        // Channel 0 Select
                    //Serial.println("Selecting loco channel 0");
                    CurrentChannel = 0;
                    ROSTERPAGE = 1;
                    RosterScreen();
                break;  

                case 14:        // Channel 1 Select
                     //Serial.println("Selecting loco channel 1");
                     CurrentChannel = 1;
                     ROSTERPAGE = 1;
                     RosterScreen();
                break;

                  
                default:
                    // do nothing
                    // the function keys are already dealt with
                break;
            }

        break;

      

        case 2:

            ButtonPressed = CheckRosterButtons();

            if (ButtonPressed >= 0 && ButtonPressed <= 8) {
                // Serial.print("Roster Item Selected = ");
                // Serial.println(ButtonPressed);

                CopyRosterItem(ButtonPressed);
                ButtonPressed = 0;
                GetFunctionKeys();      // Get the functions keys
                MainScreen();
                

            }

        
            if (ButtonPressed == 9){     // Address key pressed
                
                KeypadScreen();       // Display the Main screen
            }
            
            if (ButtonPressed == 10 ) {
                ROSTERPAGE = ROSTERPAGE - 1;
                RosterScreen();
            }

            if (ButtonPressed == 11 ) {
                ROSTERPAGE = ROSTERPAGE + 1;
                RosterScreen();
            }
            

        break;

        case 3:

            ButtonPressed = CheckNumberPad();

            if (ButtonPressed == 10){
                //This means the OK key has been pressed so use the number
                if (ADDRESS != 0){
                    LocoAddress[CurrentChannel] = ADDRESS;
                    
                    sprintf(LocoName[CurrentChannel] , "%s", "Unassigned");
                    // perhaps check that this is not already in the roster.
                    //
                    MainScreen();
                }
            }

            if (ButtonPressed == 11) {
                // This is the cancel key so forget the number input
                LocoAddress[CurrentChannel] = 0;
                sprintf(LocoName[CurrentChannel] , "%s", " ");

                MainScreen();

            }

        break;

    }

    //delay(200);

}