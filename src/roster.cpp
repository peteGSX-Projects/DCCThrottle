
#include <arduino.h>

#ifndef FILESTRUCT_H 
  #include "filestruct.h"
#endif
#include "DCCWifi.h"
#include "roster.h"


void ParseRoster(){  
 
    int ind1; // , locations
    int ind2;
    int ind3;
    int ind4;

      //readString = "<J 7309 \"LMS Jinty\">";
      
      Serial.print("captured String is : "); 
      Serial.println(readString); //prints string to serial port out

      ind1 = readString.indexOf(' ');  //finds location of first SPACE
      ind2 = readString.indexOf(' ', ind1+1 );   //finds location of second ,
      
      String StringAddress = readString.substring(ind1+1 , ind2);   //captures address
      ADDRESS = StringAddress.toInt();
      
      ind3 = readString.indexOf('"', ind2+1 );
      ind4 = readString.indexOf('"', ind3+1 );
      
      // String tempstr = readString.substring(ind3+1, ind4); //captures remain part of data after last ,
      // //int z = length(tempstr);
      // tempstr.toCharArray(LOCONAME, 18); //captures remain part of data after last ,
      LOCONAME = readString.substring(ind3+1, ind4); //captures remain part of data after last 

      Serial.print("Loco Address : ");
      Serial.println(StringAddress);
      Serial.println(ADDRESS);
      Serial.print("Loco Name : ");
      Serial.println(LOCONAME);
      
}

void ParseKeys(){

// now the Fkeys

int ind1; // , locations
int ind2;
int ind3;
int ind4;
int ind5;
int ind6;

//String readString = "";
//readString = "<J 7309 \"LMS Jinty\" \"Light/Sound/Brake/*Whistle/*Whistle/QS/*Coal/Blower/Blower/*Guard/*Injector/*Dcock/Aux/*Tannoy/*Wheel/*Noise/*Doors/SetOff/*Whistle/*Couple/Shunt/*Blower/*Buffer/*Whistle/*Whistle/F25/F26/*Vol -/*Vol +\">";

//readString = "<J 7309 \"LMS Jinty\" \"Light/Sound/Rhubarb/*Cruise/*Carrot/QS\">";

String RosterFkeys = "";

      Serial.print("captured String is : "); 
      Serial.println(readString); //prints string to serial port out

      ind1 = readString.indexOf(' ');  //finds location of first SPACE
      ind2 = readString.indexOf(' ', ind1+1 );   //finds location of second ,
      
      String StringAddress = readString.substring(ind1+1 , ind2);   //captures address
      ADDRESS = StringAddress.toInt();
      
      ind3 = readString.indexOf('"', ind2+1 );
      ind4 = readString.indexOf('"', ind3+1 );
      LOCONAME = readString.substring(ind3+1, ind4); //captures remain part of data after last ,
      //String tempstr = readString.substring(ind3+1, ind4); //captures remain part of data after last ,
      //int z = length(tempstr);
      //tempstr.toCharArray(LOCONAME, z); //captures remain part of data after last ,
      

      Serial.print("Loco Address : ");
      Serial.println(StringAddress);
      Serial.println(ADDRESS);
      Serial.print("Loco Name : ");
      Serial.println(LOCONAME);

      ind5 = ind4 + 2;
      ind6 = readString.indexOf('"', ind5+1 );
      // Serial.println(ind5);
      // Serial.println(ind6);

      RosterFkeys = readString.substring( ind5, ind6);
      Serial.println(RosterFkeys);

      ind1 = 1;
      ind2 = 0;
      bool finished = false;
      String KeyDesc;
      int keynumber = 0;
      bool Sticky = true;
      int x = CurrentChannel;
      int y = 0;

      while (finished == false){
          
          ind2 = RosterFkeys.indexOf('/', ind1+1);
          
          KeyDesc = RosterFkeys.substring(ind1, ind2);

          //int Asterix = KeyDesc.indexOf('\*');
          if (KeyDesc.startsWith("*",0)) {
            KeyDesc = KeyDesc.substring(1,KeyDesc.length());
            if (KeyDesc.length() > 7) {
              KeyDesc = KeyDesc.substring(0,6); // Strip off extra chars.
            }
            Sticky = false;
            
          }
          else {
            Sticky = true;
          }

          RosterFKeys[x][y] = KeyDesc;
          FKeyLock[x][y] = Sticky;

          Serial.print("FKey - ");
          Serial.print(keynumber);
          Serial.print(" - ");
          Serial.print(KeyDesc);
          Serial.print("  ");
          Serial.print(ind1);
          Serial.print("  ");
          Serial.print(ind2);

          if (Sticky == false) {
              Serial.println("  Non Sticky");
            }
          else {
              Serial.println("  Sticky");
            }

          keynumber++;

          ind1 = ind2+1;
          if (keynumber >28){
            finished = true;
          }
          if (ind2 <= 0 ){
            finished = true;
            
            Serial.print("Number of F Keys = ");
            Serial.println(keynumber);
            break;
          }

          if (keynumber < 28) {
              for (int z = keynumber+1; z<29; z++){
                //fill in the rest of the keys with blank
                RosterFKeys[x][z] = "\0";
                FKeyLock[x][z] = false;
              }

          }
      
        
      }
}
void CopyDefaults(){

}
   
void GetRoster(){

  bool locosfound = false;
  // issue command for CS to send the roster over.
  DoDCCRoster();

  locosfound = GetTheRoster(); // Read in the data from the CS.  If result is true then data is read.

  if (!locosfound) {
    CopyDefaults();
  }
}

void GetFunctionKeys() {

  bool fkeysfound = false;

  // issue command to get the function keys
  DoDCCFunctionKeys(CurrentChannel);

  fkeysfound = GetTheFKeys();

  if (!fkeysfound) {
    readString = DefaultRosterFkeys;
  }
  
  ParseKeys(); 
    
}
