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
//#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
#define LABEL1_FONT &FreeSansBold12pt7b    // Key label font 2
#define LABEL2_FONT &FreeSans12pt7b    // Key label font 2
#define ROTARY_FONT &FreeSerif18pt7b    // display encoder values
#define NAME_FONT &FreeSans9pt7b
#define DIRECTION_FONT &FreeSansOblique9pt7b   // display encoder values
#define ADDRESS_FONT &FreeSansBold12pt7b // display address

//tft.setFreeFont(LABEL2_FONT);


// Numeric display box size and location
#define DISP_X 100
#define DISP_Y 90
#define DISP_W 120
#define DISP_H 60
#define DISP_TSIZE 3
#define DISP_TCOLOR TFT_CYAN

// variables for the keypad.
#define NUM_LEN 5
char numberBuffer[NUM_LEN + 1] = "";
uint8_t numberIndex = 0;
int returnvalue;

// We have a status line for messages
#define STATUS_X 160 // Centred on this
#define STATUS_Y 450

int FKEYPAGE = 1; // the default start page

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
char keypadLabel[13][7] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "Bsp", "OK", "Cancel"};
uint16_t keypadColor[13] = {
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_LIGHTGREY, TFT_GREEN, TFT_RED
                        };
                                     

// Invoke the TFT_eSPI button class and create all the button objects
TFT_eSPI_Button key[15];
TFT_eSPI_Button RKey[8];
TFT_eSPI_Button NumberKey[12];
TFT_eSPI_Button AddrKey;
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
  delay(10);
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
  delay(10);
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
  delay(10);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.fillRect(0, 155, 320, 25, TFT_BLUE);
  //tft.setTextDatum(TC_DATUM);
  tft.drawString(string, 20, 158, 1); // Font 4 for fast drawing with background
  tft.drawNumber(LAddress, 240, 158);
}
//------------------------------------------------------------------------------------------


void TFTSetup() {

  // Initialise the TFT screen
  tft.init();

  // Set the rotation before we calibrate
  tft.setRotation(2);

  // Calibrate the touch screen and retrieve the scaling factors
  //touch_calibrate();
  // Replace SPIFFS file read with manual calibration
  //uint16_t calData[5] = { 490, 3200, 320, 3400, 0 };
  uint16_t calData[5] = { 361, 3100, 327, 3307, 6 }; // new screen
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

    //Serial.print("Address forChannel");
    //Serial.println(Channel);


    case 0:
    
      tft.setFreeFont(ADDRESS_FONT);
      delay(10);

      tft.fillRoundRect(1, 1, 80, 40, 3, TFT_RED);

      tft.setTextColor(TFT_WHITE, TFT_RED);   
      tft.setCursor(10, 10);
      tft.drawNumber(LocoAddress[Channel],10, 5, 2);

      tft.setTextFont(GLCD);
      delay(10);
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
      delay(10);

      tft.fillRoundRect(240, 1, 80, 40, 3, TFT_RED);
      tft.setTextColor(TFT_WHITE,TFT_RED);    
      tft.setCursor(10, 10);
      tft.drawNumber(LocoAddress[Channel],250,5, 2);

      //Serial.print("Channel 1 address ");
      //Serial.println(LocoAddress[Channel]);

      tft.setCursor(15, 220);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.setTextFont(GLCD);
      delay(10);
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
      delay(10);

      tft.setTextColor(TFT_WHITE, TFT_BLACK);   
    
      tft.drawNumber(LocoSpeed[Channel],10, 90, 4);
      
      //Serial.print("Channel 0 Speed - ");
      //Serial.println(LocoSpeed[Channel]);


      //tft.setFreeFont(DIRECTION_FONT);
      tft.setTextSize(2);

      if (LocoDirection[Channel] == 1)
      {
      
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.drawString("Fwd", 105 ,95, 2);
      }
      else 
      {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.drawString("Rev", 105,95 ,2);
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
      delay(10);
  
      tft.drawNumber(LocoSpeed[Channel],230, 90, 4);
      //Serial.print("Channel 1 Speed - ");
      //Serial.println(LocoSpeed[Channel]);

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

int MAXPAGES = MAXLOCOS/8 +1;
Serial.print("MAx Roster Pages - ");
Serial.println(MAXPAGES);


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
    case 4:
        StartPos = 21;
        EndPos = 28;
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
    strlcpy(rosterKey[KPos], ROSTER[pos], 25);
    KPos++;
  }
  
  
  // Draw the keys
  for (uint8_t row = 0; row <= (EndPos - StartPos); row++) {
  
      uint8_t b = row;
      //Serial.print("Button Row ");
      //Serial.println(row);

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

      // Adjust button label Y delta to centre of key
    
      RKey[b].setLabelDatum(0, 5);

      RKey[b].drawButton();

    
  }

  char keyLabel[] = "Addr.";

  AddrKey.initButton(&tft, 
            60,       // x
            440,    //KEY_Y + row * (KEY_H + KEY_SPACING_Y),  // x, y, w, h, outline, fill, text
            80,        // key width
            40,        // key height
            TFT_WHITE,    // outline colour
            TFT_RED,  //rkeyColour[b],  // fill colour
            TFT_WHITE,    // text colour
            keyLabel,  // text
            KEY_TEXTSIZE); // text size
  AddrKey.setLabelDatum(0, 5);
  AddrKey.drawButton();   

  
  if (ROSTERPAGE > 1) {
  
    strcpy(keyLabel, "Back");

    UpKey.initButton(&tft, 160, 440, 80, 40,  TFT_WHITE,    // outline colour
              TFT_GREEN,  TFT_WHITE, keyLabel, KEY_TEXTSIZE); // text size
    UpKey.setLabelDatum(0, 5);
    UpKey.drawButton();
  }

  // Serial.print("Maxpages ");
  // Serial.println(MAXPAGES);
  // Serial.print("ROSTERPAGE ");
  // Serial.println(ROSTERPAGE);

  if (MAXPAGES > 1 && ROSTERPAGE < MAXPAGES){

    strcpy(keyLabel, "Next");

    DownKey.initButton(&tft, 260, 440, 80, 40,  TFT_WHITE,    // outline colour
              TFT_GREEN,  TFT_WHITE, keyLabel, KEY_TEXTSIZE); // text size
    DownKey.setLabelDatum(0, 5);
    DownKey.drawButton();
  }

} // End of Roster Display
//-----------------------------------------------------------------------------

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
  if (FKEYPAGE == 3) {
    strcpy(keyLabel[7], " ");
    strcpy(keyLabel[8], " ");
    strcpy(keyLabel[9], " ");
    strcpy(keyLabel[10], " ");
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
      key[b].setLabelDatum(0, 5);
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
  int KEY_W = 85; // Width and height
  int KEY_H = 40;
  int KEY_SPACING_X = 18; // X and Y gap
  int KEY_SPACING_Y = 20;
  int KEY_TEXTSIZE = 1;   // Font size multiplier

  

  // Clear the screen
  //tft.fillScreen(TFT_BLACK);

  // Draw keypad background
  tft.fillRect(0, 80, 320, 400, TFT_DARKGREY);

  // Draw number display area and frame
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);

  // Draw the keys
  for (uint8_t row = 0; row < 4; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;

      if (b > 10) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);
      delay(10);

      NumberKey[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keypadColor[b], TFT_WHITE,
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

  delay(10);

  ShowAddress(0);
  delay(10);
  ShowAddress(1);
  delay(10);

  ShowSpeed(0);
  delay(10);
  
  ShowSpeed(1);
  delay(10);
  
   // Display which loco we are dealing with
  header("Function Keys for : ", LocoAddress[CurrentChannel]);

  DrawKeypad();

//ShowSpeed(0);

}
//-------------------------------------------------------------
void RosterScreen(){

  CURRENTSCREENPAGE = 2;

  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("<- ROSTER ->", CENTRE, 10,2);

  DrawRosterList();

}
//-------------------------------------------------------------

void KeypadScreen(){

  CURRENTSCREENPAGE = 3;

  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("ENTER ADDRESS", CENTRE, 10,2);

  DrawNumberPad();

  numberIndex = 0; // initialise the buffer counterchar numberBuffer[NUM_LEN + 1] = "";
  numberBuffer[0] = NULL;

}

int CheckButtons() {

  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
  int ReturnValue = 0; // value to send back - the key pressed
  int FunctionKey = 0; // value for DCCfunction

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
          
        } // ENDIF
        DrawKeypad();
      } // ENDIF

      if (b == 12) {
        
        Serial.println("Select Button Pressed for Channel 0");
        // do something here - Handled in loop
      } // ENDIF

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

        FunctionKey = (b + ((FKEYPAGE-1) * 11));
        //Serial.print("Function Key F");
        //Serial.println(b);
        DoDCCFunction(FunctionKey, 1);
        delay(500);  // allow time for the sound before turning off.

        
        if (StickyKeys[b] == true) // this is a sticky key so dont turn it off
        {
          if (StickyOnOff[b] == true) // Its aleady on so turn it off
            {
              DoDCCFunction(FunctionKey, 0);
              StickyOnOff[b] = false;
            } // ENDIF
            else // It wasn't on so leave it on.
            {
              StickyOnOff[b] = true;
            } // ENDESLE
        } // ENDIF
        else
        {
          DoDCCFunction(FunctionKey, 0);
        } // ENDELSE
      
      } // ENDIF

      delay(10); // UI debouncing

      ReturnValue = b;

    } // ENDIF
  } // ENDIF

  return ReturnValue;

} // END FUNCTION

//------------------------------------------------------------------------------------------

int CheckRosterButtons() {

  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
  int ReturnValue = -1; // value to send back - the key pressed -1 if no key press

  // Pressed will be set true is there is a valid touch on the screen
  bool pressed = tft.getTouch(&t_x, &t_y);

  // / Check if any key coordinate boxes contain the touch coordinates
  for (uint8_t b = 0; b < 8; b++) {
      if (pressed && RKey[b].contains(t_x, t_y)) {
        RKey[b].press(true);  // tell the button it is pressed
      } 
      else {
        RKey[b].press(false);  // tell the button it is NOT pressed
      }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 8; b++) {

    if (RKey[b].justReleased()) {
        
        RKey[b].drawButton();     // draw normal  

    }

    if (RKey[b].justPressed()) {
      
      RKey[b].drawButton(true);  // draw invert
    
        // if (b <= 10) {        // That was a roster key so display the data

        //   Serial.print("Roster Key ");
        //   Serial.println(b);
        
        // }

        delay(10); // UI debouncing

      ReturnValue = b; // this is the array index entry.

      return ReturnValue;
    }
  }   // End for

  // If we get here no function key pressed.

  // now check the Exit Key
  if (pressed && AddrKey.contains(t_x, t_y)) {
          AddrKey.press(true);  // tell the button it is pressed
        } 
        else {
          AddrKey.press(false);  // tell the button it is NOT pressed
        }
  if (AddrKey.justReleased()) {
        AddrKey.drawButton();     // draw normal  
    }
  if (AddrKey.justPressed()) {
      AddrKey.drawButton(true);  // draw invert
        ReturnValue = 9;
        //Serial.print("Exit Key ");
        
      }    

  // Now check the Up key

if (pressed && UpKey.contains(t_x, t_y)) {
          UpKey.press(true);  // tell the button it is pressed
        } 
        else {
          UpKey.press(false);  // tell the button it is NOT pressed
        }
  if (UpKey.justReleased()) {
        UpKey.drawButton();     // draw normal  
    }
  if (UpKey.justPressed()) {
      UpKey.drawButton(true);  // draw invert
        ReturnValue = 10;
        //Serial.print("Up Key ");
       
      }    

  // now check the Down key
  if (pressed && DownKey.contains(t_x, t_y)) {
          DownKey.press(true);  // tell the button it is pressed
        } 
        else {
          DownKey.press(false);  // tell the button it is NOT pressed
        }
  if (DownKey.justReleased()) {
        DownKey.drawButton();     // draw normal  
    }
  if (DownKey.justPressed()) {
      AddrKey.drawButton(true);  // draw invert
        ReturnValue = 11;
        //Serial.print("Down Key ");
        
      }    

  return ReturnValue;

} // End Function

//------------------------------------------------------------------------------------------


int CheckNumberPad() {

  // uses NumberKey[] array
  returnvalue = 0;


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
  for (uint8_t b = 0; b < 12; b++) {

    if (b > 8) tft.setFreeFont(LABEL1_FONT);
    else tft.setFreeFont(LABEL2_FONT);
    delay(10);

    if (NumberKey[b].justReleased()) NumberKey[b].drawButton();     // draw normal

    if (NumberKey[b].justPressed()) {
      NumberKey[b].drawButton(true);  // draw invert

      // if a numberpad button, append the relevant # to the numberBuffer
      if (b <= 8) {
        if (numberIndex < NUM_LEN) {
          numberBuffer[numberIndex] = keypadLabel[b][0];
          numberIndex++;
          numberBuffer[numberIndex] = 0; // zero terminate
          // Serial.print("Number key index ");
          // Serial.println(numberIndex);
          // Serial.println(keypadLabel[b]);
          returnvalue = b;
        }
      
      statusmessage(""); // Clear the old status
      }

      // Del button, so delete last char
      if (b == 9) {
        //numberBuffer[numberIndex] = NULL;
        if (numberIndex > 0) {

          char tempBuffer[NUM_LEN + 1] = "";  
          numberIndex = numberIndex -1;
          
          strncpy(tempBuffer, numberBuffer, numberIndex);
          strcpy(numberBuffer, tempBuffer);

          // Serial.println("BSP key");
          // Serial.println(numberBuffer);
          // Serial.println(numberIndex);
          // Serial.println(keypadLabel[b]);

          returnvalue = b;
        }
      }

      if (b == 10) {
        
        // this is the OK key.  Setup the address
        ADDRESS = atoi(numberBuffer);

        // Serial.println(numberBuffer);
        // Serial.println("OK key"); 
        // Serial.println(keypadLabel[b]);
        // Serial.println(ADDRESS);
        returnvalue = b;
      }

      
      if (b == 11) {
        
        // Serial.println("Cancel key"); 
        // Serial.println(keypadLabel[b]);
        
        returnvalue = b;
        
      }

      // Update the number display field
      tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
      tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
      delay(10);
      tft.setTextColor(DISP_TCOLOR);     // Set the font colour

      // Draw the string, the value returned is the width in pixels 10 changed to 5
      int xwidth = tft.drawString(numberBuffer, DISP_X + 10, DISP_Y + 12);

      // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
      // but it will not work with italic or oblique fonts due to character overlap.
      tft.fillRect(DISP_X + 10 + xwidth, DISP_Y + 1, DISP_W - xwidth - 15, DISP_H - 2, TFT_BLACK);

      delay(10); // UI debouncing
    }

  }
  
  return returnvalue;
  
}
