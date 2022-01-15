#ifndef DCCWIFI_H  
#define DCCWIFI_H  

#include <WiFi.h>
#include <SPI.h>

extern int LocoAddress[2];
extern char LocoName[2][18];
extern int LocoDirection[2];
extern int LocoSpeed[2];
extern int CurrentChannel;
extern int FunctionPressed;
//extern char incomingstring[] = "";


void WiFiSetup();
void DoDCCThrottle(int Channel);
void DoDCCFunction(int fx, int onof);

void GetTheInput();
void DoDCCRoster();

void DoDCCFunctionKeys(int Channel);

void DoDCCPower(int onof);

#endif