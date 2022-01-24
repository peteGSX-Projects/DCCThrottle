#ifndef FILESTRUCT_H  
#define FILESTRUCT_H 

//#include <Arduino.h>

struct LOCO_ROSTER  
{
  unsigned int address;
  String description;
};

extern LOCO_ROSTER LOCOS[];

struct LOCAL_ROSTER
{
  unsigned int address;
  String description[18];
};

extern LOCAL_ROSTER LOCALLOCOS[];     // Prime with defaults


extern String DefaultRosterFkeys;
extern String RosterFKeys[2][29];
    
extern bool FKeyLock[2][29];

extern int MAXLOCOS;
extern int LocoAddress[2];
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

#endif