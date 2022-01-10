
// Initial ESP32 test

#include <Arduino.h>

#include "DCCThrottle.h"
#include "display.h"
#include "rotary.h"
#include "DCCWifi.h"

int ButtonPressed;


void setup()
{
    Serial.begin(115200);

    TFTSetup();             // initialise the TFT
    StartEncoders();        // initialise the rotary encoderes
    WiFiSetup();            // Setup the WiFi connection

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

    
    
    //Serial.println(readBattery());
    

}
