#ifndef ROSTER_H
#define ROSTER_H  

#include <arduino.h>

// extern int MaxDefaults;
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

void ParseRoster();
void ParseKeys();
void CopyDefaultRoster();
void GetRoster();
void GetFunctionKeys();

#endif