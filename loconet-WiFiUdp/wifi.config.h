#pragma once

/*
  https://docs.arduino.cc/libraries/wifi
  https://docs.arduino.cc/retired/library-examples/wifi-library/ConnectWithWPA
*/



// Used for Connect and ReConnect
void WifiConnect() {

  WiFi.mode(WIFI_STA);
  Serial.printf("Attempting to connect to WPA SSID: %s .", MYSSID);
  WiFi.begin(MYSSID, MYSSID_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   printWiFiStatus();
}

//Set broadcast to local broadcast ip e.g. 192.168.0.255
void setBroadcastIP() {
  ipBroad = WiFi.localIP();
  wifiaddr = ipBroad[3];
  ipBroad[3] = 255;
}
  
void connectMDNS() {
  if (MDNS.begin(MDNS_NAME)) {
    Serial.printf("\nMDNS-Responder gestartet: %s.\n", MDNS_NAME);
  }
}

// print your WiFi shield's IP address:
void printIP() {
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
// print your MAC address:
void printMAC() {
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("WiFi-MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}

// print the SSID of the network you're attached to:
void printSSID() {
  Serial.printf("connected to %s.\n", WiFi.SSID());
}

// print the MAC address of the router you're attached to:
void printBSSID() {
  byte bssid[6];
  WiFi.BSSID(bssid);

  Serial.print("MAC address of the router (BSSID): ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);
}

// print the received signal strength:
void printRSSI() {
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
}  


void printWiFiStatus() {
  printMAC();
  if (WiFi.status() == WL_CONNECTED) {
    printSSID();
    printRSSI(); 
    printIP();
    printBSSID();
 
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("--------------- WiFi NOT Connected --------------");
  }
}


void Setup_Wifi() {
  Serial.println("Start with Wifi Setup");

  if (WiFi.status() == WL_NO_SHIELD) {
    char msg[] = "WiFi shield not present";
    Serial.println(msg);
    while (true)
      ;  // Stop here, if no WiFi Shield is present.
  }
 
  WifiConnect();
  connectMDNS();
}
 
//re-connect wifi if not connected
void Loop_Wifi() {


  if (WiFi.status() != WL_CONNECTED) {
    Serial.println();
    Serial.print("WIFI-Connection Lost. re-Connecting .");
    WifiConnect();
  }
}