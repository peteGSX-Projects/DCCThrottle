#ifndef DISPLAY_H  
#define DISPLAY_H  

// extern struct LOCO_ROSTER  
// {
//   unsigned int address;
//   String description;
// };

// extern LOCO_ROSTER LOCOS[];

// extern struct LOCAL_ROSTER
// {
//   unsigned int address;
//   String description[18];
// };
// extern LOCAL_ROSTER LOCALLOCOS[];   // Prime with defaults

extern int LocoAddress[2];
extern int MAXLOCOS;
extern char LocoName[2][18];
extern int LocoDirection[2];
extern int LocoSpeed[2];
extern int CurrentChannel;
extern int FunctionPressed;
extern bool PowerOn;
extern bool SoundOn[2];
extern String readString;
extern int ADDRESS; //loco address
extern String LOCONAME;

extern String DefaultRosterFkeys;



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