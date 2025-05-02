

/*
  https://docs.arduino.cc/language-reference/en/functions/wifi/udp/
  https://docs.arduino.cc/retired/library-examples/wifi-library/WiFiUdpSendReceiveString/
*/ 

#ifndef LOCONET_UDP_PORT
#define LOCONET_UDP_PORT 32001;
#endif
const int loconet_udp_port = LOCONET_UDP_PORT;       // UDP Port

WiFiUDP udpRead;                              // Read
WiFiUDP udpSend;                              // Write
char ReplyBuffer[] = "acknowledged";                 // a string to send back

void debugUDP() {
  Serial.println();
  Serial.println("------------------ UDP-DUMP ------------------");
  Serial.print("Received packet of size ");
  Serial.println(rec_packetSize);
  Serial.print("From ");
  Serial.print(remoteIp);
  Serial.print(":");
  Serial.print(remotePort);

  Serial.print("Contents:  ");
  dump_byte_array(rec_packetBuffer, rec_packetLen);
  Serial.println();
  Serial.println("------------------ UDP-DUMP ------------------");
}

void UDPFetch() {
  rec_packetSize = udpRead.parsePacket();
  if (rec_packetSize) {

    rec_packetLen = udpRead.read(rec_packetBuffer, 255);
    if (rec_packetLen > 0) {
      rec_packetBuffer[rec_packetLen] = 0;
    }
    debugUDP();
    remoteIp = udpRead.remoteIP();
    remotePort = udpRead.remotePort();
    udpRead.flush();



    
    // send a reply, to the IP address and port that sent us the packet we received
    udpSend(udpRead.remoteIP(), udpRead.remotePort());
    udpSend.write(ReplyBuffer);
    udpSend.endPacket();
    /**/
  }
}

void Setup_UDP() {

  udpRead.begin(loconet_udp_port);
  udpSend.begin(loconet_udp_port);
}