

#include <WiFi.h>
#include <WiFiClient.h>  
#include <ESPmDNS.h>  
#include <vector>

#include "secrets.h";    // SSID and Key
#include "config.h";     // other Config
#include "variables.h";     // global Variables
#include "loconet.util.h";     // 

#include "wifi.config.h";     // Connect to Wifi

#include "tcp.server.h";     // Loconet TCP-Server.CONFIG 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    while (!Serial) {

    ;  // wait for serial port to connect. Needed for native USB port only
  }
  
  Setup_Wifi();
setupTCPServer();
}

void loop() {
  
  LoopTimer = micros();
  // Check if WiFi Connection was lost
  Loop_Wifi();
loopTCPServer();

}
