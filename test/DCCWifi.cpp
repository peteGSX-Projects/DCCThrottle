#include <Arduino.h>

#include "DCCWiFi.h"


char ssid[] = "DCCEX_c25024";          //  your network SSID (name)
char pass[] = "Monarch1466";   // your network password
char host[] = "192.168.4.1";
int port = 2560;

// We've declared the WiFi credentials in the main sketch
extern const char *SSID;
extern const char *PASSWORD;
//extern const char *host[];
//extern const int *port;

//int status = 0;
bool wiFiDisconnected = true;
WiFiClient DCCclient;

// Current state of WiFi (connected, idling)
int status = WL_IDLE_STATUS;

// WiFi specific defines
#define WIFITIMEOUTSECONDS 20


void WiFiSetup() {

  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  //WiFi.mode(WIFI_STA);

  // dont allow WiFitosleep
  //WiFi.setSleep(false);

  WiFi.begin(ssid, pass);

  // try a few times to connect
  int timeout = WIFITIMEOUTSECONDS * 4;

  Serial.print("WiFi Status ");
  Serial.println(WiFi.status());

  while (WiFi.status() != WL_CONNECTED && (timeout-- > 0))
  {
      delay(500);
      WiFi.reconnect();
      delay(500);
      Serial.print(".");
  }

  //Succesful connection?

  wl_status_t wifiStatus = WiFi.status();
  if (wifiStatus != WL_CONNECTED)
  {
      Serial.println("/nFailed to connect, exiting.");
      //Serial.printf("WiFi Status: %s\n", wl_status_to_string(wifiStatus));
      return;
  }  

  Serial.printf("WiFi connected with (local) IP address of: %s\n", WiFi.localIP().toString().c_str());
	wiFiDisconnected = false;

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 

  Serial.print("Now connecting to DCC++EX Server : ");
  Serial.println(host);

  WiFiClient DCCclient;

  Serial.println("Connected to wifi");
  Serial.println("\nStarting connection...");
  // if you get a connection, report back via serial:
  if (DCCclient.connect("192.168.4.1", 2560)) {
    Serial.println("connected");
    
  } 
      

}


// Convert the WiFi (error) response to a string we can understand
// const char * wl_status_to_string(wl_status_t status)
// {

// 	switch (status)
  
// 	{
// 	case WL_NO_SHIELD:
// 		return "WL_NO_SHIELD";
// 	case WL_IDLE_STATUS:
// 		return "WL_IDLE_STATUS";
// 	case WL_NO_SSID_AVAIL:
// 		return "WL_NO_SSID_AVAIL";
// 	case WL_SCAN_COMPLETED:
// 		return "WL_SCAN_COMPLETED";
// 	case WL_CONNECTED:
// 		return "WL_CONNECTED";
// 	case WL_CONNECT_FAILED:
// 		return "WL_CONNECT_FAILED";
// 	case WL_CONNECTION_LOST:
// 		return "WL_CONNECTION_LOST";
// 	case WL_DISCONNECTED:
// 		return "WL_DISCONNECTED";
// 	default:
// 		return "UNKNOWN";
// 	}  
  
// }


void DoDCCThrottle(int Channel) {
//if (debug == 1) Serial.println(LocoDirection[ActiveAddress] );
  //DCCclient.print("<1>");

  Serial.print("<t 1 ");
  Serial.print(LocoAddress[Channel] );//locoID);
  Serial.print(" ");
  Serial.print(LocoSpeed[Channel] );
  Serial.print(" ");
  Serial.print(LocoDirection[Channel]);
  Serial.println(">");

  DCCclient.print("<t 1 ");
  DCCclient.print(LocoAddress[Channel] );//locoID);
  DCCclient.print(" ");
  DCCclient.print(LocoSpeed[Channel] );
  DCCclient.print(" ");
  DCCclient.print(LocoDirection[Channel]);
  DCCclient.println(">");
}

void DoDCCFunction(int fx, int onof){

  Serial.print("<F ");
  Serial.print(LocoAddress[CurrentChannel]);
  Serial.print(" ");
  Serial.print(fx);
  Serial.print(" ");
  Serial.print(onof);
  Serial.println(">");

  DCCclient.print("<F ");
  DCCclient.print(LocoAddress[CurrentChannel]);
  DCCclient.print(" ");
  DCCclient.print(fx);
  DCCclient.print(" ");
  DCCclient.print(onof);
  DCCclient.println(">");
}

void DoDCCPower(int onof){

  Serial.print("<");
  Serial.print(onof);
  Serial.println(">");

  // DCCclient.write("<");
  // DCCclient.print(onof);
  // DCCclient.print(">");
}