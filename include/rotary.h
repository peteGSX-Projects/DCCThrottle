#ifndef ROTARY_H
#define ROTARY_H


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

void StartEncoders();
void CheckEncoders();



#endif