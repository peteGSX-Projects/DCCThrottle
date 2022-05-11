#ifndef DCCTHROTTLE_H 
#define DCCTHROTTLE_H  

struct LOCO_ROSTER  
{
  unsigned int address;
  char description[15];
  
}  LOCOS[] = {
    { 3, "Jinty" },
    { 5, "Castle" },
    { 7, "King" },
    { 9, "28xx" },
    { 11, "Hornby Star" },
    { 0, "Jinty"},
    { 0, "Jinty"},
    { 0, "Jinty" },
    { 0, "Jinty" },
    { 0, "Jinty" }
    };

char KeyLabel[29][6] =
{ 
  "Light",
  "Sound",
  "Brake",
  "Whist",
  "Whist",
  "QS",
  "Coal",
  "F7",
  "F8",
  "F9",
  "F10",
  "F11",
  "F12",
  "F13",
  "F14",
  "F15",
  "F16",
  "F17",
  "F18",
  "F19",
  "F20",
  "F21",
  "F22",
  "F23",
  "F24",
  "F25",
  "F26",
  "F27",
  "F28"
  };


int LocoAddress[2] = {3,5};
char LocoName[2][15] = {"Jinty","Castle"};
int LocoDirection[2] = {1, 1};
int LocoSpeed[2] = {0, 0};
int CurrentChannel = 0;
int FunctionPressed = 0;

#endif