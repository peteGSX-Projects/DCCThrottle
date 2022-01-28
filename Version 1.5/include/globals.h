#ifndef GLOBALS_H  
#define GLOBALS_H  

extern int LocoAddress[2];
extern int MAXLOCOS;
extern int MaxDefaults;
extern char LocoName[2][18];
extern int LocoDirection[2];
extern int LocoSpeed[2];
extern int CurrentChannel;
extern int FunctionPressed;
extern bool PowerOn;
extern bool SoundOn[2];
extern String readString;
extern int ADDRESS; //loco address
extern char LOCONAME[18];

extern String DefaultRosterFkeys;

extern String RosterFKeys[2][29];
    
extern bool FKeyLock[2][29];


#endif