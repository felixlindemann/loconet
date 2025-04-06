
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>


#include "secrets.h";    // SSID and Key
#include "config.h";     // other Config
#include "OPC.h";        // OPC COnstant definition
#include "variables.h";  // global variables
#include "SV.h";         // Storeable Variables

#include "helper.h";       // Functions
#include "wifi.config.h";  // Wifi COnfig
#include "loconet.udp.h";  // actual Loconet Program


void setup() {
  Serial.begin(115200);
  LEDCOUNT =  sizeof(ledPins) / sizeof(int);
  while (!Serial) {

    ;  // wait for serial port to connect. Needed for native USB port only
  }
  Setup_Wifi();
  Setup_UDP();
 
  for(int i =0; i<   LEDCOUNT; i++){
     pinMode(ledPins[i], OUTPUT);
     digitalWrite(ledPins[i], HIGH);
     delay(1000);
  }

  
  for(int i =0; i<   LEDCOUNT; i++){
     digitalWrite(ledPins[i], LOW);
     delay(1000);
  }
}

void loop() {
  // Check if WiFi Connection was lost
  Loop_Wifi();
  UDPFetch();  //have we recieved data??
  switch (rec_packetSize) {
    case 0:  // no rx data to work with //if not receiving, do other things...
      break;

    case 4:  //
      locoNet4(rec_packetBuffer);
    case 9:
      locoNet9(rec_packetBuffer);
    case 2:  //
    case 14:
    case 16:
    default:
      break;
  }
  delay(1);
}
