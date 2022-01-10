# DCCThrottle
ESP32 Based WiFI Throttle for DCC++EX

This project was started to consider the possibilty of creating a WiFi based throttle 
using rotary knobs that was capable of communicating with the DCC++EX.  The project uses
and ESP32 WROOM development board with built in WIFI.  The ESP32 was used in place of
the ESP8266 to gain the benefit of additional I/O pins.

This is very much a work-in-progress. The project compiles and runs under VSCODE and 
Platformio, however currently on my laptop compiling under Arduino IDE produces a problem
with duplicate WiFI libraries.

The **TFT_eSPI** library by **BODMER** is required to control the TFT LCD screen.  
The User_Setup.hfile included here should be copied to the TFT_e_SPI library directory.  
This defines the setup for the TFT LCD display.

Two rotary encoders are used to provide two throttle channels.

The code to monitor the Rotary Encoders was provided by **Ralph Bacon** based on theory from
https://www.pinteric.com/rotary.html

**Current Situation**

The project currently has the following limitations
1. The loco address for each of the two channels is currently hard coded.
2. A ROSTER data structure is provided to define  range of loco's.
3. The function key assignments for each channel are currently hard coded.
4. Only one page of function keys (F0 to F8) is displayed for each channel.
5. The function key page can be switched between channels.
6. The concept of operating two locos simultaneously is proven to work.

**Planned Enhancements** 

1. The DCC++EX development team have added a ROSTER feature to the CS.  Hopefully
in the next release commands will be available to download this data to a throttle.
2. A selection screen will be written to select the required locos from a ROSTER list.
3. Function key assignemnts will be downloaded from the CS when a loco is seleced.
4. The function key pages will be extended to display all function keys for each channel
5. Battery monitoring will be added.

**Please note**

It is not intended to add CV programming to this throttle.  For this function I use a
second CS connected to JMRI.





