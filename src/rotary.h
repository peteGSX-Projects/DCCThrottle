#ifndef ROTARY_H
#define ROTARY_H

extern int LocoAddress[2];
extern char LocoName[2][15];
extern int LocoDirection[2];
extern int LocoSpeed[2];
extern int CurrentChannel;
extern int FunctionPressed;

void IRAM_ATTR rotary1();
void IRAM_ATTR rotary2();

void StartEncoders();
void CheckEncoders();



#endif
