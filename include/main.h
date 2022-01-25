#ifndef MAIN_H  
#define MAIN_H  



int MAXLOCOS = 0;
int LocoAddress[2] = {7309,5275};
char LocoName[2][18] = {"Jinty","42xx 2-8-0 Tank"};
int LocoDirection[2] = {1, 1};
int LocoSpeed[2] = {0, 0};
int CurrentChannel = 0;
int FunctionPressed = 0;
bool PowerOn = true;
bool SoundOn[2] = {false, false};
String readString = "";
int ADDRESS; //loco address
String LOCONAME;

String DefaultRosterFkeys = "Light/Sound/Brake/*Whistle/*Whistle/QS/*Coal/Blower/Blower/*Guard/*Injector/*Dcock/Aux/*Tannoy/*Wheel/*Noise/*Doors/SetOff/*Whistle/*Couple/Shunt/*Blower/*Buffer/*Whistle/*Whistle/F25/F26/*Vol -/*Vol +";
String RosterFKeys[2][29];
    
bool FKeyLock[2][29];





#endif