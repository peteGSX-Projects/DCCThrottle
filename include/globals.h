#ifndef GLOBALS_H  
#define GLOBALS_H  

extern char ROSTER[20][30];
extern char LOCALROSTER[20][30];


extern int MAXLOCOS;
extern int MAXDEFAULTLOCOS;
extern int LocoAddress[2];
extern char LocoName[2][25];
extern int LocoDirection[2];
extern int LocoSpeed[2];
extern int CurrentChannel;
extern int FunctionPressed;
extern bool PowerOn;
extern bool SoundOn[2];
extern String readString;
extern int ADDRESS; //loco address
extern char LOCONAME[30];
extern bool WiFiConnected;
extern int CURRENTSCREENPAGE;
extern int ROSTERPAGE;

extern String DefaultRosterFkeys;

extern String RosterFKeys[2][29];
    
extern bool FKeyLock[2][29];


#endif