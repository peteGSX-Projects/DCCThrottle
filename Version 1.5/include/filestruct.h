#ifndef FILESTRUCT_H  
#define FILESTRUCT_H 

#include <Arduino.h>

struct LOCO_ROSTER  
{
  unsigned int address;
  char description[18];
};


struct LOCAL_ROSTER
{
  unsigned int address;
  char description[18];
};

extern struct LOCO_ROSTER LOCOS[5];

extern struct LOCAL_ROSTER LOCALLOCOS[5] = {    // Prime with defaults
  { 7309, "LMS Jinty"},
  { 5275, "42xx 2-8-0 Tank"},
  { 3433, "City of Bath"},
  { 6110, "Large Prarie"},
  { 7246, "72xx 2-8-2 Tank"} 
};


#endif