/*
  The TFT_eSPI library incorporates an Adafruit_GFX compatible
  button handling class, this sketch is based on the Arduin-o-phone
  example.

  This example diplays a keypad where numbers can be entered and
  send to the Serial Monitor window.

  The sketch has been tested on the ESP8266 (which supports SPIFFS)

  The minimum screen size is 320 x 240 as that is the keypad size.

  TOUCH_CS and SPI_TOUCH_FREQUENCY must be defined in the User_Setup.h file
  for the touch functions to do anything.
*/

// The SPIFFS (FLASH filing system) is used to hold touch screen
// calibration data

//#include "FS.h"

#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library


#include "Free_Fonts.h" // Include the header file attached to this sketch

#include "globals.h"
#include "display.h"

#include "filestruct.h"

#include "DCCWiFi.h"


TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// define centre of screen
#define CENTRE 160

// Function Keypad start position, key sizes and spacing
#define KEY_X 60 // Centre of key
#define KEY_Y 210
#define KEY_W 80 // Width and height
#define KEY_H 40
#define KEY_SPACING_X 18 // X and Y gap
#define KEY_SPACING_Y 20
#define KEY_TEXTSIZE 2   // Font size multiplier

// Roster Keypad start position, key sizes and spacing
#define RKEY_X 160 // Centre of key
#define RKEY_Y 80
#define RKEY_W 300 // Width and height
#define RKEY_H 40
#define RKEY_SPACING_X 10 // X and Y gap
#define RKEY_SPACING_Y 10
#define RKEY_TEXTSIZE 2   // Font size multiplier

// Using two fonts since numbers are nice when bold
#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
//#define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2
#define LABEL2_FONT &FreeSans12pt7b    // Key label font 2
#define ROTARY_FONT &FreeSans12pt7b    // display encoder values
#define DIRECTION_FONT &FreeSansOblique9pt7b    // display encoder values

// Numeric display box size and location
#define DISP_X 1
#define DISP_Y 10
#define DISP_W 238
#define DISP_H 50
#define DISP_TSIZE 3
#define DISP_TCOLOR TFT_CYAN

// Number length, buffer for storing it and character index
#define NUM_LEN 12
char numberBuffer[NUM_LEN + 1] = "";
uint8_t numberIndex = 0;


// We have a status line for messages
#define STATUS_X 160 // Centred on this
#define STATUS_Y 450

int FKEYPAGE = 1; // the default start page
int ROSTERPAGE = 1;

// Create Key Array for Roster List
char rosterKey[10][30];
uint16_t Rkeycolour[10] = {TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE};

// Create 15 default keys for the funtion keypad
char keyLabel[15][7] = {
                       "F0", "Sound", "Brake", "Wh 1", "Wh 2", "QS",
                       "Coal", "Blower", "Clank", "Guard", "Inj", "Page",
                       "Menu", "Power", "Swap" 
                       };
uint16_t keyColor[15] = {TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_MAGENTA,
                         TFT_DARKGREY, TFT_GREEN, TFT_DARKGREY
                        };

// defien sticky keys, i.e. they stay on until pushed off
bool StickyKeys[15] = { true, true, true, false, false, 
                        true, false, false, false, false,
                        false, false, false, false, false};   
bool StickyOnOff[15] = { false, false, false, false, false, 
                        false, false, false, false, false,
                        false, false, false, false, false};                                             

// Invoke the TFT_eSPI button class and create all the button objects
TFT_eSPI_Button key[15];

//------------------------------------------------------------------------------------------

// Print something in the mini status bar
void statusmessage(const char *msg) 
{
  tft.setTextPadding(240);
  tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.setTextFont(0);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1);
  tft.drawString(msg, STATUS_X, STATUS_Y);
}

//------------------------------------------------------------------------------------------
// Print something on the screen
void screenmessage(const char *msg) 
{
  
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.setTextFont(0);
  
  tft.setTextSize(1);
  tft.print(msg);
  tft.println();

}

//------------------------------------------------------------------------------------------
// Print the split screen header for a display screen
void header(const char *string, int LAddress)
{
  tft.setTextSize(2);
  tft.setTextFont(0);
  tft.setTextColor(TFT_MAGENTA, TFT_BLUE);
  tft.fillRect(0, 155, 320, 25, TFT_BLUE);
  //tft.setTextDatum(TC_DATUM);
  tft.drawString(string, 20, 158, 1); // Font 4 for fast drawing with background
  tft.drawNumber(LAddress, 240, 158);
}
//------------------------------------------------------------------------------------------


void TFTSetup() {
  // Use serial port
  //Serial.begin(115200);

  // Initialise the TFT screen
  tft.init();

  // Set the rotation before we calibrate
  tft.setRotation(0);

  // Calibrate the touch screen and retrieve the scaling factors
  //touch_calibrate();
  // Replace SPIFFS file read with manual calibration
  uint16_t calData[5] = { 490, 3200, 320, 3400, 0 };
  tft.setTouch(calData);


  tft.println("Starting Screen");
  delay(1000);

  // Clear the screen
  tft.fillScreen(TFT_BLACK);

  // tft.setTextColor(TFT_WHITE);
  // tft.drawCentreString("<- Loco ->", CENTRE, 10,4);

 
  
}

void ShowAddress(int Channel){

  switch (Channel)
  {

    Serial.print("Address forChannel");
    Serial.println(Channel);

    case 0:
    

      tft.fillRoundRect(1, 1, 80, 40, 3, TFT_RED);

      tft.setTextColor(TFT_WHITE, TFT_RED);   
      tft.setCursor(10, 10);
      tft.drawNumber(LocoAddress[Channel],10, 10,4);

      //Serial.print("Channel 0 address ");
      //Serial.println(LocoAddress[Channel]);

      tft.setCursor(15, 50);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      
      //tft.print(LocoName[Channel]);
      tft.drawString(LocoName[Channel], 10,50);

      break;

    case 1:
      

      tft.fillRoundRect(240, 1, 80, 40, 3, TFT_RED);
      tft.setTextColor(TFT_WHITE,TFT_RED);    
      tft.setCursor(10, 10);
      tft.drawNumber(LocoAddress[Channel],250,10,4);

      //Serial.print("Channel 1 address ");
      //Serial.println(LocoAddress[Channel]);

      tft.setCursor(15, 210);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);

      tft.drawRightString(LocoName[Channel], 305, 50, 2);

      break;
  }


}

//------------------------------------------------------------------------------------------

void ShowSpeed(int Channel){


 switch (Channel)
  {
    case 0:
    
      tft.fillRoundRect(1, 85, 95, 60, 5, TFT_BLACK);
      tft.drawRoundRect(1, 85, 95, 60, 5, TFT_RED);
      //tft.setFreeFont(ROTARY_FONT);
      tft.setTextSize(2);

      tft.setTextColor(TFT_WHITE, TFT_BLACK);   
      //tft.setCursor(10, 90);
      tft.drawNumber(LocoSpeed[Channel],10, 90, 4);
      Serial.print("Channel 0 Speed - ");
      Serial.println(LocoSpeed[Channel]);


      //tft.setFreeFont(DIRECTION_FONT);
      tft.setTextSize(2);

      if (LocoDirection[Channel] == 1)
      {
      
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.drawString("Fwd", 100 ,95, 2);
      }
      else 
      {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.drawString("Rev", 100,95 ,2);
      }

      break;

    case 1:
      
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      //tft.setFreeFont(DIRECTION_FONT);
      tft.setTextSize(2);

      if (LocoDirection[Channel] == 1)
      {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.drawString("Fwd", 165,95 ,2);
      }
      else
      {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.drawString("Rev", 165,95 ,2);
      }
     
      tft.fillRoundRect(220, 85, 100, 60, 3, TFT_BLACK);
      tft.drawRoundRect(220, 85, 100, 60, 3, TFT_RED);

      tft.setTextColor(TFT_WHITE, TFT_BLACK);   
      
      //tft.setTextSize(4);
      //tft.setFreeFont(ROTARY_FONT);
      tft.drawNumber(LocoSpeed[Channel],230, 90, 4);
      Serial.print("Channel 1 Speed - ");
      Serial.println(LocoSpeed[Channel]);

      break;

  }
}


//------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------
void DrawRosterList()
{
  // Create the FKey array for display

  // char keyLabel[15][7] = {
  //                      "F0", "Sound", "Brake", "Wh 1", "Wh 2", "QS",
  //                      "Coal", "Blower", "Clank", "Guard", "Inj", "Page",
  //                      "Menu", "Power", "Swap" 
  //                      };
  // String RosterFKeys[2][29];
  //int FKEYPAGE = 1;

  int StartPos = 0;
  int EndPos = 0;
  switch (ROSTERPAGE){
    case 1:
        StartPos = 0;
        EndPos = 9;
        break;
    case 2:
        StartPos = 10;
        EndPos = 19;
        break;

  }

  String tempkey;
  int KPos = 0;
  for (uint8_t pos = StartPos; pos <= EndPos; pos++) {
    tempkey = char(LOCOS[pos].address);
    tempkey = + " ";
    tempkey = tempkey + String(LOCOS[pos].description);
    tempkey.toCharArray(rosterKey[pos], 30);
    //strcpy(rosterKey[pos], tempkey)
    KPos++;
  }
  // strcpy(keyLabel[12], "Page");
  // strcpy(keyLabel[13], "Menu");
  // strcpy(keyLabel[14],"Power");
  // strcpy(keyLabel[15],"Swap");
  
  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      //if (b < 3) tft.setFreeFont(LABEL1_FONT);
      //else 
      //tft.setFreeFont(LABEL2_FONT);
      tft.setTextSize(4);

      key[b].initButton(&tft, RKEY_X + col * (RKEY_W + RKEY_SPACING_X),
                        RKEY_Y + row * (RKEY_H + RKEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        RKEY_W, RKEY_H, TFT_WHITE, Rkeycolour[b], TFT_WHITE,
                        rosterKey[b], RKEY_TEXTSIZE);
      key[b].drawButton();
    }
  }
}

void DrawKeypad()
{
  // Create the FKey array for display

  // char keyLabel[15][7] = {
  //                      "F0", "Sound", "Brake", "Wh 1", "Wh 2", "QS",
  //                      "Coal", "Blower", "Clank", "Guard", "Inj", "Page",
  //                      "Menu", "Power", "Swap" 
  //                      };
  // String RosterFKeys[2][29];
  //int FKEYPAGE = 1;

  int StartPos = 0;
  int EndPos = 0;
  switch (FKEYPAGE){
    case 1:
        StartPos = 0;
        EndPos = 10;
        break;
    case 2:
        StartPos = 11;
        EndPos = 21;
        break;
    case 3:
        StartPos = 22;
        EndPos = 29;
        break;
  }

  String tempkey;
  int KPos = 0;
  for (uint8_t pos = StartPos; pos <= EndPos; pos++) {
    tempkey = RosterFKeys[CurrentChannel][pos];
    tempkey.toCharArray(keyLabel[KPos],7);
    KPos++;
  }
  strcpy(keyLabel[12], "Page");
  strcpy(keyLabel[13], "Menu");
  strcpy(keyLabel[14],"Power");
  strcpy(keyLabel[15],"Swap");
  
  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      //if (b < 3) tft.setFreeFont(LABEL1_FONT);
      //else 
      //tft.setFreeFont(LABEL2_FONT);
      tft.setTextSize(4);

      key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel[b], KEY_TEXTSIZE);
      key[b].drawButton();
    }
  }
}




//---------------------------------------

void MainScreen() {

  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("<- Loco ->", CENTRE, 10,4);

  ShowAddress(0);
  ShowAddress(1);

  ShowSpeed(0);
  
  ShowSpeed(1);
  
   // Display which loco we are dealing with
  header("Function Keys for : ", LocoAddress[CurrentChannel]);

  DrawKeypad();

//ShowSpeed(0);

}
//-------------------------------------------------------------
void RosterScreen(){
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("<- ROSTER ->", CENTRE, 10,4);

  DrawRosterList();

}
//-------------------------------------------------------------


int CheckButtons() {

  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
  int ReturnValue; // value to send back - the key pressed

  // Pressed will be set true is there is a valid touch on the screen
  bool pressed = tft.getTouch(&t_x, &t_y);

  // / Check if any key coordinate boxes contain the touch coordinates
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && key[b].contains(t_x, t_y)) {
      key[b].press(true);  // tell the button it is pressed
    } else {
      key[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    // if (b < 3) tft.setFreeFont(LABEL1_FONT);
    // else tft.setFreeFont(LABEL2_FONT);

    if (key[b].justReleased()) {
      if (StickyKeys[b] == false)
      {
      key[b].drawButton();     // draw normal  
      }
      else // sticky key os true - is it already on?
      {
        if (StickyOnOff[b] == false){ // it's off so re-draw the key
          key[b].drawButton();
        }
        
      }

    }

    if (key[b].justPressed()) {
      key[b].drawButton(true);  // draw invert
    
      //Page button, so draw next page
      if (b == 11) {
        Serial.println("Page Button Pressed");
        // do something here
      }

      if (b == 12) {
        
        Serial.println("Loco Button Pressed");
        // do something here
      }
      
      if (b == 13) {
        
        Serial.println("Power Button Pressed");
        if (PowerOn == true)
        {
          DoDCCPower(0);
          PowerOn = false;
          key[b].drawButton(TFT_RED);
        }
        else 
        {
          DoDCCPower(1);
          PowerOn = true;
        }
      }

      if (b == 14) {
        
        Serial.println("Swap Button Pressed");
        if (CurrentChannel == 0)
        {
          CurrentChannel = 1;
        }
        else
        {
          CurrentChannel = 0;
        }
         // Display which loco we are dealing with
        header("Function Keys for : ", LocoAddress[CurrentChannel]);

      }

      if (b <= 10) {        // That was a function key so send the signal

        Serial.print("Function Key F");
        Serial.println(b);
        DoDCCFunction(b, 1);
        delay(1000);  // allow time for the sound before turning off.

        if (StickyKeys[b] == true) // this is a sticky key so dont turn it off
        {
          if (StickyOnOff[b] == true) // Its aleady on so turn it off
            {
              DoDCCFunction(b, 0);
              StickyOnOff[b] = false;
            }
            else // It wasn't on so leave it on.
            {
              StickyOnOff[b] = true;
            }
        }
        else
        {
          DoDCCFunction(b, 0);
        }
      
      }

      delay(10); // UI debouncing

      ReturnValue = b;
      return ReturnValue;


    }
  }
}

//------------------------------------------------------------------------------------------

