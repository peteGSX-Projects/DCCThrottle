#ifndef MAIN_H  
#define MAIN_H  

struct LOCO_ROSTER  
{
  unsigned int address;
  char description[15];
  char KeyLabel[29][6];
} LOCOS[] = {
    { 3, "Jinty", {"Light","Sound","Brake","Whist","Whist","QS","Coal","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28"}},
    { 5, "Castle", {"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28"}},
    { 7, "King", {"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28"}},
    { 9, "28xx", {"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28"}},
    { 11, "Hornby Star", {"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28"}},
    { 0, "Jinty", {"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28"}},
    { 0, "Jinty", {"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28"}},
    { 0, "Jinty", {"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28"}},
    { 0, "Jinty", {"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28"}},
    { 0, "Jinty", {"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28"}}
    };


struct DCC_ROSTER
{
  unsigned int address;
  char description[16];
} DCCLOCOS[] = {};

struct LOCAL_ROSTER
{
  unsigned int address;
  char description[18];
} LOCALLOCOS[] = {        // Prime with defaults
  { 7309, "LMS Jinty"},
  { 5275, "42xx 2-8-0 Tank"},
  { 3433, "City of Bath"},
  { 6110, "Large Prarie"},
  { 7246, "72xx 2-8-2 Tank"} 
};


char DefaultRosterFkeys[] = "Light/Sound/Brake/*Whistle/*Whistle/QS/*Coal/Blower/Blower/*Guard/*Injector/*Dcock/Aux/*Tannoy/*Wheel/*Noise/*Doors/SetOff/*Whistle/*Couple/Shunt/*Blower/*Buffer/*Whistle/*Whistle/F25/F26/*Vol -/*Vol +";
char RosterFkeys[2][29][8] = {
     {"Light","Sound","Brake","Whist","Whist","QS","Coal","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28"},
     {"Light","Sound","Brake","Whist","Whist","QS","Coal","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28"}
};

int LocoAddress[2] = {7309,5275};

int LocoDirection[2] = {1, 1};
int LocoSpeed[2] = {0, 0};
int CurrentChannel = 0;
char LocoName[2][18] = {"Jinty","42xx 2-8-0 Tank"};
int FunctionPressed = 0;
bool PowerOn = true;
bool SoundOn[2] = {false, false};



#endif