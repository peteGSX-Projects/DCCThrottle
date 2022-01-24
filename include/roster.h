#ifndef ROSTER_H
#define ROSTER_H  

#include <arduino.h>

extern LOCO_ROSTER LOCOS[];
extern LOCAL_ROSTER LOCALLOCOS[];     // Prime with defaults

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

void ParseRoster();
void ParseKeys();
void CopyDefaults();
void GetRoster();
void GetFunctionKeys();

#endif