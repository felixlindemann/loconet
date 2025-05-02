#ifndef TCP_SERVER_INCLUDED
#define TCP_SERVER_INCLUDED

/* This is the TCP port used to communicate over ethernet 
   LoconetOverTCP defaults to PORT 1024 */
#ifndef LOCONET_TCP_PORT
#define LOCONET_TCP_PORT 1234
#endif

/* LoconetOverTCP defaults to PORT 1024 */
WiFiServer server(LOCONET_TCP_PORT);
boolean gotATCPMessage = false;  // whether or not you got a message from the client yet
 
 

#ifdef DEBUG
static void dumpPacket(char* dPacket); 
#endif

void setupTCPServer() {
 
  /*start listening for clients */
  server.begin();
  Serial.println("LoconetOverTCP buffer started!!");
}

void loopTCPServer() {
  WiFiClient client = server.available();
 if (client) {
    Serial.println("TCP-Client verbunden");

    while (client.connected()) {
      // TCP → LocoNet UART
      if (client.available()) { 
         
        std::vector<uint8_t> packet;
        bool chksum = false;
        Serial.print("Data received: ");
        while (client.available() && !chksum) {
          uint8_t v = client.read();
          dump_byte(v);
          packet.push_back(v);
          chksum = isValidLocoNetChecksum(packet.data());

        }
    Serial.println();
        //if(chksum ==true){ 
 
          decodeLocoNetPacket(packet.data(), packet.size());
          // }
           client.write(packet.data(), packet.size());
      }
 
    }
 
  }
delay(1); 
}

 

#endif  // #ifndef TCP_SERVER_INCLUDED