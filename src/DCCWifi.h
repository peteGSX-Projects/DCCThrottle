#ifndef DCCWIFI_H  
#define DCCWIFI_H  

#include <WiFi.h>
#include <SPI.h>

extern int LocoAddress[2];
extern char LocoName[2][15];
extern int LocoDirection[2];
extern int LocoSpeed[2];
extern int CurrentChannel;
extern int FunctionPressed;


void WiFiSetup();
//const char * wl_status_to_string(wl_status_t status);
void DoDCCThrottle(int Channel);
void DoDCCFunction(int fx, int onof);
void DoDCCPower(int onof);

#endif