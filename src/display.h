#ifndef DISPLAY_H  
#define DISPLAY_H  

extern int LocoAddress[2];
extern char LocoName[2][15];
extern int LocoDirection[2];
extern int LocoSpeed[2];
extern int CurrentChannel;
extern int FunctionPressed;
extern bool PowerOn;
extern bool SoundOn[2];


void statusmessage(const char *msg);
void screenmessage(const char *msg);
void header(const char *string);

void TFTSetup();
void ShowAddress(int Channel);
void ShowSpeed(int Channel);
void DrawKeypad();
void MainScreenHeader();
int CheckButtons();

//#include <TFT_eSPI.h>      // Hardware-specific library
//TFT_eSPI tft = TFT_eSPI(); // Invoke custom library


#endif