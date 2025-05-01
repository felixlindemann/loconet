#include <WiFi.h>
#include <WiFiClient.h> 
#include <ESPmDNS.h> 


#include "secrets.h";    // SSID and Key
#include "config.h";     // other Config
#include "variables.h";     // other Config
#include "wifi.config.h";     // other Config

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    while (!Serial) {

    ;  // wait for serial port to connect. Needed for native USB port only
  }
  
  Setup_Wifi();

}

void loop() {
  
  LoopTimer = micros();
  // Check if WiFi Connection was lost
  Loop_Wifi();


}
