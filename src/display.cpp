/*
  The TFT_eSPI library incorporates an Adafruit_GFX compatible
  button handling class, this sketch is based on the Arduin-o-phone
  example.

  This example diplays a keypad where numbers can be entered and
  send to the Serial Monitor window.


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

//#include "filestruct.h"

#include "DCCWiFi.h"


TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// define centre of screen
#define CENTRE 160

// Using two fonts since numbers are nice when bold
#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
//#define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2
#define LABEL2_FONT &FreeSans12pt7b    // Key label font 2
#define ROTARY_FONT &FreeSerif18pt7b    // display encoder values
#define NAME_FONT &FreeSans9pt7b
#define DIRECTION_FONT &FreeSansOblique9pt7b   // display encoder values
#define ADDRESS_FONT &FreeSansBold12pt7b // display address

//tft.setFreeFont(LABEL2_FONT);


// Numeric display box size and location
#define DISP_X 120
#define DISP_Y 85
#define DISP_W 80
#define DISP_H 50
#define DISP_TSIZE 3
#define DISP_TCOLOR TFT_CYAN

#define NUM_LEN 6
char numberBuffer[NUM_LEN + 1] = "";
uint8_t numberIndex = 0;

// We have a status line for messages
#define STATUS_X 160 // Centred on this
#define STATUS_Y 450

int FKEYPAGE = 1; // the default start page
int ROSTERPAGE = 1;

// Create Key Array for Roster List
char rosterKey[9][25];
uint16_t rkeyColour[9] = {TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE,
                          TFT_MAGENTA};


// Create 15 default keys for the funtion keypad
char keyLabel[15][7] = {
                       "F0", "Sound", "Brake", "Wh 1", "Wh 2", "QS",
                       "Coal", "Blower", "Clank", "Guard", "Inj", "Page",
                       "Select", "Swap", "Select" 
                       };
uint16_t keyColor[15] = {TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_MAGENTA,
                         TFT_GOLD, TFT_GREEN, TFT_GOLD
                        };

// defien sticky keys, i.e. they stay on until pushed off
bool StickyKeys[15] = { true, true, true, false, false, 
                        true, false, false, false, false,
                        false, false, false, false, false};   
bool StickyOnOff[15] = { false, false, false, false, false, 
                        false, false, false, false, false,
                        false, false, false, false, false};        

// Create 12 keys for the Number keypad
char keypadLabel[12][7] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "Bsp", "OK", "Cancel"};
uint16_t keypadColor[12] = {
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_LIGHTGREY, TFT_GREEN, TFT_RED
                        };
                                     

// Invoke the TFT_eSPI button class and create all the button objects
TFT_eSPI_Button key[15];
TFT_eSPI_Button RKey[10];
TFT_eSPI_Button NumberKey[12];
TFT_eSPI_Button ExitKey;
TFT_eSPI_Button UpKey;
TFT_eSPI_Button DownKey;

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
  
  tft.setTextSize(2);
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

  char lloconame[16] = "";
  int namelen = 0;

  switch (Channel)
  {

    Serial.print("Address forChannel");
    Serial.println(Channel);


    case 0:
    
      tft.setFreeFont(ADDRESS_FONT);

      tft.fillRoundRect(1, 1, 80, 40, 3, TFT_RED);

      tft.setTextColor(TFT_WHITE, TFT_RED);   
      tft.setCursor(10, 10);
      tft.drawNumber(LocoAddress[Channel],10, 10, 2);

      tft.setTextFont(GLCD);
      //tft.setFreeFont(NAME_FONT);
      tft.setCursor(15, 50);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      
      if (sizeof(LocoName[Channel]) > 12){
          namelen = 12;
      }
      else {
          namelen = sizeof(LocoName[Channel]);
      }
      strncpy(lloconame, LocoName[Channel], namelen);
      tft.drawString(lloconame, 10, 50 ,1);
      //tft.print(LocoName[Channel]);

      break;

    case 1:
      
      tft.setFreeFont(ADDRESS_FONT);
      
      tft.fillRoundRect(240, 1, 80, 40, 3, TFT_RED);
      tft.setTextColor(TFT_WHITE,TFT_RED);    
      tft.setCursor(10, 10);
      tft.drawNumber(LocoAddress[Channel],250,10, 2);

      //Serial.print("Channel 1 address ");
      //Serial.println(LocoAddress[Channel]);

      tft.setCursor(15, 220);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.setTextFont(GLCD);
      //tft.setFreeFont(NAME_FONT);
      if (sizeof(LocoName[Channel]) > 12){
          namelen = 12;
      }
      else {
          namelen = sizeof(LocoName[Channel]);
      }
      strncpy(lloconame, LocoName[Channel], namelen);
      
      tft.drawRightString(lloconame, 305, 50, 1);

      break;
  }


}

//------------------------------------------------------------------------------------------

void ShowSpeed(int Channel){

  //tft.setTextSize(2);

 switch (Channel)
  {
    case 0:
    
      tft.fillRoundRect(1, 85, 95, 60, 5, TFT_BLACK);
      tft.drawRoundRect(1, 85, 95, 60, 5, TFT_RED);
      tft.setFreeFont(ROTARY_FONT);

      tft.setTextColor(TFT_WHITE, TFT_BLACK);   
    
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
      tft.setFreeFont(ROTARY_FONT);

  
      tft.drawNumber(LocoSpeed[Channel],230, 90, 4);
      Serial.print("Channel 1 Speed - ");
      Serial.println(LocoSpeed[Channel]);

      break;

  }
}


//------------------------------------------------------------------------------------------

void DrawRosterList()
{

CURRENTSCREENPAGE = 2;

  // Roster Keypad start position, key sizes and spacing
int KEY_X = 160; // Centre of key
int KEY_Y = 80;
int KEY_W =300; // Width and height
int KEY_H = 35;
//int KEY_SPACING_X = 10; // X and Y gap
int KEY_SPACING_Y = 10;
int KEY_TEXTSIZE = 2;   // Font size multiplier



  int StartPos = 0;
  int EndPos = 0;
  switch (ROSTERPAGE){
    case 1:
        StartPos = 0;
        EndPos = 7;
        break;
    case 2:
        StartPos = 8;
        EndPos = 15;
        break;
    case 3:
        StartPos = 16;
        EndPos = 20;
        break;
  }

  if (EndPos > MAXLOCOS) {
      EndPos = MAXLOCOS - 1;
  }
  
  Serial.println("Drawing Roster Keys");
  Serial.print("End Pos = ");
  Serial.println(EndPos);

  String tempkey;
  int KPos = 0;
  for (uint8_t pos = StartPos; pos <= EndPos; pos++) {
    Serial.println(ROSTER[pos]);
    //strlcpy(rosterKey[KPos], ROSTER[pos], strlen(ROSTER[pos]) + 1);
    strlcpy(rosterKey[KPos], ROSTER[pos], 25);
    KPos++;
  }
  
  
  // Draw the keys
  for (uint8_t row = 0; row <= EndPos; row++) {
  
      uint8_t b = row;
      Serial.print("Button Row ");
      Serial.println(row);

      tft.setTextSize(2);

      RKey[b].initButton(&tft, 
            KEY_X,       // x
            KEY_Y + row * (KEY_H + KEY_SPACING_Y),  // x, y, w, h, outline, fill, text
            KEY_W,       // key width
            KEY_H,        // key height
            TFT_WHITE,    // outline colour
            TFT_BLUE,  //rkeyColour[b],  // fill colour
            TFT_WHITE,    // text colour
            rosterKey[b],  // text
            KEY_TEXTSIZE); // text size

      RKey[b].drawButton();

    
  }

  char keyLabel[] = "Back";

  ExitKey.initButton(&tft, 
            60,       // x
            440,    //KEY_Y + row * (KEY_H + KEY_SPACING_Y),  // x, y, w, h, outline, fill, text
            80,        // key width
            35,        // key height
            TFT_WHITE,    // outline colour
            TFT_RED,  //rkeyColour[b],  // fill colour
            TFT_WHITE,    // text colour
            keyLabel,  // text
            KEY_TEXTSIZE); // text size

  ExitKey.drawButton();   

  
  strcpy(keyLabel, "Up");

  UpKey.initButton(&tft, 160, 440, 80, 35,  TFT_WHITE,    // outline colour
            TFT_GREEN,  TFT_WHITE, keyLabel, KEY_TEXTSIZE); // text size
  UpKey.drawButton();

  strcpy(keyLabel, "Down");

  UpKey.initButton(&tft, 260, 440, 80, 35,  TFT_WHITE,    // outline colour
            TFT_GREEN,  TFT_WHITE, keyLabel, KEY_TEXTSIZE); // text size
  UpKey.drawButton();

}

void DrawKeypad()
{

  //CURRENTSCREENPAGE = 1;
  
  // Function Keypad start position, key sizes and spacing
  int KEY_X = 60; // Centre of key
  int KEY_Y = 210;
  int KEY_W =80; // Width and height
  int KEY_H = 40;
  int KEY_SPACING_X = 18; // X and Y gap
  int KEY_SPACING_Y = 20;
  int KEY_TEXTSIZE = 2;   // Font size multiplier

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
  strcpy(keyLabel[11], "Page");
  strcpy(keyLabel[12], "Select");
  strcpy(keyLabel[13],"Swap");
  strcpy(keyLabel[14],"Select");
  
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

//------------------------------------------------------------------------------------------
void DrawNumberPad()
{

  CURRENTSCREENPAGE = 3;

  // Function Keypad start position, key sizes and spacing
  int KEY_X = 60; // Centre of key
  int KEY_Y = 210;
  int KEY_W = 80; // Width and height
  int KEY_H = 40;
  int KEY_SPACING_X = 18; // X and Y gap
  int KEY_SPACING_Y = 20;
  int KEY_TEXTSIZE = 1;   // Font size multiplier

 

  // Clear the screen
  //tft.fillScreen(TFT_BLACK);

  // Draw keypad background
  tft.fillRect(0, 80, 320, 400, TFT_DARKGREY);

  // Draw number display area and frame
  tft.fillRect(130, 90, 80, 50, TFT_BLACK);
  tft.drawRect(130, 90, 80, 50, TFT_WHITE);

  // Draw the keys
  for (uint8_t row = 0; row < 4; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b > 9) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      NumberKey[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keypadLabel[b], KEY_TEXTSIZE);
      NumberKey[b].drawButton();
    }
  }
}
//------------------------------------------------------------------------------------------

void MainScreen() {

  CURRENTSCREENPAGE = 1;

  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.drawCentreString("<- Loco ->", CENTRE, 10, 2);

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
  tft.drawCentreString("<- ROSTER ->", CENTRE, 10,2);

  DrawRosterList();

}
//-------------------------------------------------------------

void KeypadScreen(){

  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("ENTER ADDRESS", CENTRE, 10,2);

  DrawNumberPad();

}

int CheckButtons() {

  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
  int ReturnValue = 0; // value to send back - the key pressed

  // Pressed will be set true is there is a valid touch on the screen
  bool pressed = tft.getTouch(&t_x, &t_y);

  // / Check if any key coordinate boxes contain the touch coordinates
  for (uint8_t b = 0; b < 15; b++) {
    if (pressed && key[b].contains(t_x, t_y)) {
      key[b].press(true);  // tell the button it is pressed
    } // ENDIF
    else {
      key[b].press(false);  // tell the button it is NOT pressed
    } // ENDELSE
  } // ENDFOR

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    // if (b < 3) tft.setFreeFont(LABEL1_FONT);
    // else tft.setFreeFont(LABEL2_FONT);

    if (key[b].justReleased()) {
      if (StickyKeys[b] == false)
      {
      key[b].drawButton();     // draw normal  
      } // ENDIF
      else // sticky key os true - is it already on?
      {
        if (StickyOnOff[b] == false){ // it's off so re-draw the key
          key[b].drawButton();
        } // ENDIF
        
      } // END ELSE

    } // ENDIF

    if (key[b].justPressed()) {
      key[b].drawButton(true);  // draw invert
    
      //Page button, so draw next page
      if (b == 11) {
        Serial.println("Page Button Pressed");
        FKEYPAGE++;
        if (FKEYPAGE > 3) {
          FKEYPAGE = 1;
          DrawKeypad();
        } // ENDIF
        
      } // ENDIF

      if (b == 12) {
        
        Serial.println("Select Button Pressed for Channel 0");
        // do something here - Handled in loop
      } // ENDIF
      
      // if (b == 13) {
        
      //   Serial.println("Power Button Pressed");
      //   if (PowerOn == true)
      //   {
      //     DoDCCPower(0);
      //     PowerOn = false;
      //     key[b].drawButton(TFT_RED);
      //   }  // ENDIF
      //   else 
      //   {
      //     DoDCCPower(1);
      //     PowerOn = true;
      //   } // ENDELSE
      // } // ENDIF

      if (b == 13) {
        
        Serial.println("Swap Button Pressed");
        if (CurrentChannel == 0)
        {
          CurrentChannel = 1;
        } // ENDIF
        else
        {
          CurrentChannel = 0;
        } // ENDELSE
         // Display which loco we are dealing with
        header("Function Keys for : ", LocoAddress[CurrentChannel]);
        FKEYPAGE = 1; // set to page 1
        DrawKeypad(); // draw the keys for this channel

      } // ENDIF

      if (b == 14) {
        
        Serial.println("Select Button Pressed for Channel 1");
        // do something here - Handled in loop
      } // ENDIF

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
            } // ENDIF
            else // It wasn't on so leave it on.
            {
              StickyOnOff[b] = true;
            } // ENDESLE
        } // ENDIF
        else
        {
          DoDCCFunction(b, 0);
        } // ENDELSE
      
      } // ENDIF

      delay(10); // UI debouncing

      ReturnValue = b;

    } // ENDIF
  } // ENDIF

  return ReturnValue;

} // END FUNCTION

void CopyRosterItem(int b){


}
//------------------------------------------------------------------------------------------

int CheckRosterButtons() {

  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
  int ReturnValue = 0; // value to send back - the key pressed

  // Pressed will be set true is there is a valid touch on the screen
  bool pressed = tft.getTouch(&t_x, &t_y);

  // / Check if any key coordinate boxes contain the touch coordinates
  for (uint8_t b = 0; b < 10; b++) {
      if (pressed && RKey[b].contains(t_x, t_y)) {
        RKey[b].press(true);  // tell the button it is pressed
      } 
      else {
        RKey[b].press(false);  // tell the button it is NOT pressed
      }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    // if (b < 3) tft.setFreeFont(LABEL1_FONT);
    // else tft.setFreeFont(LABEL2_FONT);

    if (RKey[b].justReleased()) {
        
        RKey[b].drawButton();     // draw normal  

    }

    if (RKey[b].justPressed()) {
      
      RKey[b].drawButton(true);  // draw invert
    
        if (b <= 10) {        // That was a roster key so display the data

          Serial.print("Roster Key ");
          Serial.println(b);
          CopyRosterItem(b);
        
        }

        delay(10); // UI debouncing

      ReturnValue = b; // this is the array index entry.

    }
  }   // End for

  return ReturnValue;

} // End Function

//------------------------------------------------------------------------------------------


bool CheckNumberPad() {

  // uses NumberKey[] array
  bool returnvalue = false;

  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates

  // Pressed will be set true is there is a valid touch on the screen
  bool pressed = tft.getTouch(&t_x, &t_y);

  // / Check if any key coordinate boxes contain the touch coordinates
  for (uint8_t b = 0; b < 12; b++) {
    if (pressed && NumberKey[b].contains(t_x, t_y)) {
      NumberKey[b].press(true);  // tell the button it is pressed
    } else {
      NumberKey[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 15; b++) {

    if (b < 3) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);

    if (NumberKey[b].justReleased()) NumberKey[b].drawButton();     // draw normal

    if (NumberKey[b].justPressed()) {
      NumberKey[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the numberBuffer
      if (b <= 10) {
        if (numberIndex < NUM_LEN) {
          numberBuffer[numberIndex] = keyLabel[b][0];
          numberIndex++;
          numberBuffer[numberIndex] = 0; // zero terminate
        }
       statusmessage(""); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 10) {
        numberBuffer[numberIndex] = 0;
        if (numberIndex > 0) {
          numberIndex--;
          numberBuffer[numberIndex] = 0;//' ';
        }
        statusmessage(""); // Clear the old status
      }

      if (b == 11) {
        statusmessage("Sent value to serial port");
        Serial.println(numberBuffer);
        returnvalue = true;
        
      }
      // we dont really check that the text field makes sense
      // just try to call
      if (b == 12) {
        statusmessage("Value cleared");
        numberIndex = 0; // Reset index to 0
        numberBuffer[numberIndex] = 0; // Place null in buffer
        returnvalue = false;
        
      }

      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels
      int xwidth = tft.drawString(numberBuffer, DISP_X + 4, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
    }
  
    if (returnvalue == true){
      ADDRESS = atoi(numberBuffer);

    }

    return returnvalue;
  }
}
