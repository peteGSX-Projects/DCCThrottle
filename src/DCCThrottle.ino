
// Initial ESP32 test

#include <Arduino.h>

#include "DCCThrottle.h"
#include "display.h"
#include "rotary.h"
#include "DCCWifi.h"

int ButtonPressed;

// String readBattery(){
//   uint8_t percentage = 100;
//   //float voltage = analogRead(35) / 4096.0 * 7.23;      // LOLIN D32 (no voltage divider need already fitted to board.or NODEMCU ESP32 with 100K+100K voltage divider
//   float voltage = analogRead(39) / 4096.0 * 7.23;    // NODEMCU ESP32 with 100K+100K voltage divider added
//   //float voltage = analogRead(A0) / 4096.0 * 4.24;    // Wemos / Lolin D1 Mini 100K series resistor added
//   //float voltage = analogRead(A0) / 4096.0 * 5.00;    // Ardunio UNO, no voltage divider required
//   Serial.println("Voltage = " + String(voltage));
//   percentage = 2808.3808 * pow(voltage, 4) - 43560.9157 * pow(voltage, 3) + 252848.5888 * pow(voltage, 2) - 650767.4615 * voltage + 626532.5703;
//   if (voltage > 4.19) percentage = 100;
//   else if (voltage <= 3.50) percentage = 0;
//   return String(percentage)+"%";
// }

void setup()
{
    Serial.begin(115200);

    TFTSetup();             // initialise the TFT
    StartEncoders();        // initialise the rotary encoderes
    WiFiSetup();            // Setup the WiFi connection


    Serial.println("Sending Roster Command");
    DoDCCRoster();          // Get the roster data from the server

    //Serial.println("Getting FKeys for Channel 0");

    DoDCCFunctionKeys(0);      // Get the functions for loco on channel 0

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