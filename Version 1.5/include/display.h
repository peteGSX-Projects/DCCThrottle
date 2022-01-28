#ifndef DISPLAY_H  
#define DISPLAY_H  


// extern int LocoAddress[2];
// extern int MAXLOCOS;
// extern char LocoName[2][18];
// extern int LocoDirection[2];
// extern int LocoSpeed[2];
// extern int CurrentChannel;
// extern int FunctionPressed;
// extern bool PowerOn;
// extern bool SoundOn[2];
// extern String readString;
// extern int ADDRESS; //loco address
// extern char LOCONAME[18];

// extern String RosterFKeys[2][29];

// extern String DefaultRosterFkeys;

void statusmessage(const char *msg);
void screenmessage(const char *msg);
void header(const char *string);

void TFTSetup();
void ShowAddress(int Channel);
void ShowSpeed(int Channel);

void DrawRosterList();
void DrawKeypad();
void MainScreen();
void RosterScreen();
int CheckButtons();


#endif