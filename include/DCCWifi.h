#ifndef DCCWIFI_H  
#define DCCWIFI_H  

#include <WiFi.h>
#include <SPI.h>

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
// extern LOCAL_ROSTER LOCALLOCOS[];     // Prime with defaults

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

extern String DefaultRosterFkeys;
extern String RosterFKeys[2][29];
    
extern bool FKeyLock[2][29];

extern LOCO_ROSTER LOCOS[];

void WiFiSetup();
void DoDCCThrottle(int Channel);
void DoDCCFunction(int fx, int onof);

bool GetTheRoster();
bool GetTheFKeys();
void DoDCCRoster();

void DoDCCFunctionKeys(int Channel);

void DoDCCPower(int onof);

#endif