#include "esp32-hal-gpio.h"

/*
  https://docs.arduino.cc/language-reference/en/functions/wifi/udp/
  https://docs.arduino.cc/retired/library-examples/wifi-library/WiFiUdpSendReceiveString/
*/


#ifndef LOCONET_UDP_PORT
#define LOCONET_UDP_PORT 1235;
#endif
const int loconet_udp_port = LOCONET_UDP_PORT;       // UDP Port
byte udpTestPacket0[] = { 0xB2, 0x02, 0x50, 0x1F };  // Testpackage 1
byte udpTestPacket1[] = { 0xB2, 0x02, 0x40, 0x0F };  // Testpackage 2
WiFiUDP LoconetUDPRead;                              // Read
WiFiUDP LoconetUDPSend;                              // Write
char ReplyBuffer[] = "acknowledged";                 // a string to send back
int rec_packetSize = 0;                              // Length of recieved Package
int rec_packetLen = 0;                               // lenguth of Package
uint8_t rec_packetBuffer[255];                       //buffer to hold incoming packet
IPAddress remoteIp;                                  // IP where package came from
int remotePort;                                      // Port where package came from

void fn_OPC_SW_REQ() {
  /*
        ;REQ SWITCH function NO
          ;<0xB0>,<SW1>,<SW2>,<CHK> REQ SWITCH function
          <SW1> =<0,A6,A5,A4- A3,A2,A1,A0>,     7 ls adr bits. A1,A0 select 1 of 4 input pairs in a DS54
          <SW2> =<0,0,DIR,ON- A10,A9,A8,A7>     Control bits and 4 MS adr bits.
              ,DIR=1 for Closed,/GREEN, =0 for Thrown/RED,ON=1 for Output ON, =0 FOR output OFF
          Note-,Immediate response of <0xB4><30><00> if command failed, otherwise no response
      */
  // ********** Look for set messages for my servo addresses
  uint16_t Address = CommandFor(rec_packetBuffer);
  uint8_t cmd = CommandData(rec_packetBuffer);
  Serial.print(F("Set Output: "));
  Serial.print(Address);
  Serial.print(" to :");
  Serial.println(cmd);
 
}

void locoNet9(uint8_t *RECMsg) {

  // E4 09 00 1F 11 00 11 00 0D
  // E4 = LocoNet OPC_SW_REQ (Switch Request)
  // 09 = Länge des Pakets
  // 00 1F = Adressierung / Portnummer
  // 11 = Steuerdaten
  // 00 11 Daten
  // 00 0D Prüfsumme
  if (RECMsg[0] == 0xE4 && RECMsg[1] == 0x09) {
    int adresse = CommandFor(RECMsg);
    uint8_t value = RECMsg[6] ;
    int led = RECMsg[6];

    int i = 0;
    // Serial.printf("\n Port: %d Value: %d", adresse, led);
    uint8_t maske = 0;

     
    for (int i = 0; i < 6; i++) { 
        digitalWrite(ledPins[i], ledPins[i] ==22? HIGH: LOW);
      }  
    delay(100);

    for (int i = 0; i < 6; i++) {
      if (value & (1 << i)) {
        digitalWrite(ledPins[i],  ledPins[i] ==22?LOW: HIGH);
      }  
    }
  }
}

void locoNet4(uint8_t *RECMsg) {
  uint16_t CVRequest;
  uint8_t CV_Data;

  switch (RECMsg[0]) {
    // Check type of MSG
    case opc_SW_REQ:  //OPC SW REQ B0 {//+++++++++++servo and output setting ++++++++++++++++++++++++++++++
      fn_OPC_SW_REQ();

      /*
        for (int i = 1; i <= 8; i++) {
          if ((CommandFor(recMessage) == SensorAddress(i)) && (((SV[3 * i] & 0x88) == 0x88))) {  //if its a ", Sensor Request", its a SERVO driver

            Serial.print(F("Set Servo: "));

            SetServo(i, CommandData(recMessage, Message_Length));
          }
          if ((CommandFor(recMessage) == SensorAddress(i)) && (((SV[3 * i] & 0x88) == 0x80))) {  //it an output command

            Serial.print(F("Set Output: "));
            Serial.print(i);
            Serial.print(" to :");
            Serial.println(CommandData(recMessage, Message_Length));

            digitalWrite(NodeMcuPortD[i], CommandData(recMessage, Message_Length));
          }



        }  // ..CommandOpCode  = OPC_SW_REQ)
      */
      break;
    case opc_INPUT_REP:   // OPC_INPUT_REP  0xB2  General sensor input codes   // no need to do anything with sensor codes..
    case opc_LONG_ACK:    // OPC_LONG_ACK  0xB4  Long acknowledge    // no need to do anything with other peoples acks..
    case opc_RQ_SL_DATA:  // OPC_RQ_SL_DATA){  BB //REQUEST SLOT DATA+++++++++++++++++++++
    case opc_LOCO_ADR:    //OPC_LOCO_ADR
    default:
      break;
  }  //end of case statements
}  // end len4 commands

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
  rec_packetSize = LoconetUDPRead.parsePacket();
  if (rec_packetSize) {

    rec_packetLen = LoconetUDPRead.read(rec_packetBuffer, 255);
    if (rec_packetLen > 0) {
      rec_packetBuffer[rec_packetLen] = 0;
    }
    // debugUDP();
    remoteIp = LoconetUDPRead.remoteIP();
    remotePort = LoconetUDPRead.remotePort();
    LoconetUDPRead.flush();



    /*
    // send a reply, to the IP address and port that sent us the packet we received
    LoconetUDPSend(LoconetUDPRead.remoteIP(), LoconetUDPRead.remotePort());
    LoconetUDPSend.write(ReplyBuffer);
    LoconetUDPSend.endPacket();
    */
  }
}

void Setup_UDP() {

  LoconetUDPRead.begin(loconet_udp_port);
  LoconetUDPSend.begin(loconet_udp_port);
}
