#ifndef MAIN_H  
#define MAIN_H  

char ROSTER[20][30];
char LOCALROSTER[20][30] = {
    "7309 LMS Jinty",
    "5275 42xx 2-8-0 Tank",
    "3433 City of Bath",
    "6110 Large Prarie",
    "7246 72xx 2-8-2 Tank"};


int MAXLOCOS = 0;
int MAXDEFAULTLOCOS = 5;
int LocoAddress[2] = {7309,5275};
char LocoName[2][25] = {"Jinty","42xx 2-8-0 Tank"};
int LocoDirection[2] = {1, 1};
int LocoSpeed[2] = {0, 0};
int CurrentChannel = 0;
int FunctionPressed = 0;
bool PowerOn = true;
bool SoundOn[2] = {false, false};
String readString = "";
int ADDRESS; //loco address
char LOCONAME[30];

String RosterFKeys[2][29];

String DefaultRosterFkeys = "<J 1234 \"Dummy\" \"Light/Sound/Brake/*Whistle/*Whistle/QS/*Coal/Blower/Blower/*Guard/*Injector/*Dcock/Aux/*Tannoy/*Wheel/*Noise/*Doors/SetOff/*Whistle/*Couple/Shunt/*Blower/*Buffer/*Whistle/*Whistle/F25/F26/*Vol -/*Vol +";

    
bool FKeyLock[2][29];





#endif