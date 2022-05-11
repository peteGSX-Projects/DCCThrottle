#ifndef DCCWIFI_H  
#define DCCWIFI_H  

#include <WiFi.h>
#include <SPI.h>




void WiFiSetup();
void DoDCCThrottle(int Channel);
void DoDCCFunction(int fx, int onof);

bool GetTheRoster();
bool GetTheFKeys();
void DoDCCRoster();

void DoDCCFunctionKeys(int Channel);

void DoDCCPower(int onof);

#endif