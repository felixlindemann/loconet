
#include <WiFi.h>
#include <WiFiClient.h> 
#include <ESPmDNS.h>


#include "secrets.h";
#include "config.h";
#include "variables.h";
#include "wifi.config.h";

void setup() {
  Serial.begin(115200);

  while (!Serial) {

    ;  // wait for serial port to connect. Needed for native USB port only
  }
  Setup_Wifi();

}

void loop() {
  // Check if WiFi Connection was lost
  Loop_Wifi();


  delay(5000);
}
