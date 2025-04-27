


/*
  https://docs.arduino.cc/language-reference/en/functions/wifi/udp/
  https://docs.arduino.cc/retired/library-examples/wifi-library/WiFiUdpSendReceiveString/
*/ 

uint8_t DIRF =0 ;
uint8_t SND =0 ;
uint16_t MyLocoLAddr = 1;
uint8_t MyLocoAddr = 1;


#ifndef LOCONET_UDP_PORT
#define LOCONET_UDP_PORT 32001;
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

/*
void setOPC_SL_RD_DATA_Msg(uint8_t *SendMsg, uint8_t SLOT, uint8_t STAT, uint8_t ADR, uint8_t SPD, uint8_t DIRF, uint8_t TRK, uint8_t SS2, uint8_t ADR2, uint8_t SND, uint8_t ID1, uint8_t ID2) {
  unsigned char k = 0;
  uint16_t tempaddr;
  SendMsg[0] = 0xE7;  //OP code Slot data read

  SendMsg[1] = 0x0E;  // count
  SendMsg[2] = SLOT;
  SendMsg[3] = STAT;
  SendMsg[4] = ADR;
  SendMsg[5] = SPD;
  SendMsg[6] = DIRF;
  SendMsg[7] = TRK;
  SendMsg[8] = SS2;
  SendMsg[9] = ADR2;
  SendMsg[10] = SND;
  SendMsg[11] = ID1;
  SendMsg[12] = ID2;
  SendMsg[13] = 0xFF;
  for (k = 0; k < 13; k++) {  //Make checksum for this three byte message
    SendMsg[13] ^= SendMsg[k];
  }
}

void SL_RD_Data(uint8_t *SendMsg) {


#if _SERIAL_DEBUG
  Serial.print(F(" Sending SL_RD_Data :"));
#endif
  // MyLocoLAddr = CV[18]+((CV[17]&0x3F)*256);  /// will be only the short address used here..  ?
  // MyLocoAddr  = CV[1];  /// should be recMsg[2]; ///FORCE KEEP MyLocoAddr

   
  setOPC_SL_RD_DATA_Msg(SendMsg, MyLocoAddr, 0x03, MyLocoAddr, Motor_Speed, DIRF, 0x03, 0x01, 0x00, SND, 0x00, 0x00);
  UDPSEND(SendMsg, 14, 2);  // 2ms delay!
}
*/
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
    uint8_t value = RECMsg[6];
    int led = RECMsg[6];

    int i = 0;
    // Serial.printf("\n Port: %d Value: %d", adresse, led);
    uint8_t maske = 0;


    for (int i = 0; i < 6; i++) {
      if (!(value & (1 << i))) {
        MCPSetLED(adresse + i, false);
      }
    }
    // delay(200);

    for (int i = 0; i < 6; i++) {
      if (value & (1 << i)) {
        MCPSetLED(adresse + i, true);
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

void Len14Commands(uint8_t *RECMsg) {
  uint16_t CVRequest;
  uint8_t CV_Data;
  if ((RECMsg[0] == OPC_WR_SL_DATA) & (RECMsg[2] == 0x7C)) {                //"Write PT slot data"   read write cv data.
                                                                            //if ((CommandFor(RECMsg)== MyLocoAddr)||(CommandFor(RECMsg)== MyLocoLAddr)||(CommandFor(RECMsg)== 0)) {  // ?? Full check of possibilities? May need to add a CV29  bit to select L and S addr messages..
    if ((CommandFor(RECMsg) == MyLocoAddr) || (CommandFor(RECMsg) == 0)) {  // Just short addres
      digitalWrite(BlueLed, LOW);                                           /// turn On
      WaitUntill = millis() + 1000;
      CVRequest = (RECMsg[9] & 0x7F) + (128 * bitRead(RECMsg[8], 0)) + (256 * bitRead(RECMsg[8], 4)) + (512 * bitRead(RECMsg[8], 5));
      CV_Data = (RECMsg[10] & 0x7F) + (128 * bitRead(RECMsg[8], 1));
      //REQUEST CV  DATA+++++++++++++++++++++
      //5<HOPSA>Operations Mode Programming- 7 High address bits of Loco to program, 0 if Service Mode
      //6<LOPSA>Operations Mode Programming- 7 Low address bits of Loco to program, 0 if Service Mode
      //7<TRK> Normal Global Track status for this Master, Bit 3 also is 1 WHEN Service Mode track is BUSY
      //8<CVH> High 3 BITS of CV#, and ms bit of DATA.7 <0,0,CV9,CV8 - 0,0, D7,CV7>
      //9<CVL> Low 7 bits of 10 bit CV address. <0,CV6,CV5,CV4-CV3,CV2,CV1,CV0>
      //10<DATA7>Low 7 BITS OF data to WR or RD COMPARE <0,D6,D5,D4 - D3,D2,D1,D0> ms bit is at CVH bit 1 position.
      if (bitRead(RECMsg[3], 6) == 0) {  // read
 
        Serial.print(F("Read request for CV["));
        Serial.print(CVRequest + 1);
        Serial.print(F("]  ="));
        Serial.print(CV[CVRequest + 1]);
 
        setOPC_LONG_ACK_Msg(sendMessage, RECMsg[0], 0x01);  // Task accepted , will send <E7> (Slot data response.)reply at completion
 
        Serial.println(F(" Sending OPC_LONG_ACK_Msg line 168 "));
 
        UDPSEND(sendMessage, 4, 2);
        //  <0xEF>,<0E>,<7C>,                     <PCMD>,<0>,<HOPSA>,<LOPSA>,<TRK>;<CVH>,<CVL>,<DATA7>,<0>,<0>,<CHK>
        BuildProgrammerResponseMessage(sendMessage, RECMsg[3], RECMsg[5], RECMsg[6], RECMsg[7], RECMsg[8], RECMsg[9], CV[CVRequest + 1]);  // basically a copy of the command but with the requested data...
 
        Serial.print("  Sending response message");
        Serial.print(" CV[");
        Serial.print(CVRequest + 1);
        Serial.print("]=");
        Serial.print(CV[CVRequest + 1]);
        // Serial.print("   Full Message =");
        //        dump_byte_array(sendMessage, 14);
        Serial.println("");
 
        UDPSEND(sendMessage, 14, 2);
      }  // end of READ

      else {  // its a write...
 
        Serial.print(F("Write CV["));
        Serial.print(CVRequest + 1);
        Serial.print(F("]  ="));
        Serial.print(CV_Data);
 
        if ((CVRequest + 1 == 8)) {  // set defaults with CV8=8
          if ((CV_Data == 8)) {
            SetDefaultSVs();
          }
        } else {
          CV[CVRequest + 1] = CV_Data;
        }
        WriteSV2EPROM();  // update EEPROM
        Data_Updated = true;
        EPROM_Write_Delay = millis() + Ten_Sec;
        //EEPROM.commit();  // done on timer now
        EPROM2SV();                                         // update the SV table
        setOPC_LONG_ACK_Msg(sendMessage, RECMsg[0], 0x01);  // acknowledge
 
        Serial.print(F("long ack"));
        //dump_byte_array(sendMessage, 0x04);
        Serial.println();
 
        UDPSEND(sendMessage, 4, 2);

      }  // end of write cv
    }    //end our MyLocoAddr
  }      //end request cv data
}  // endLen14Commands
/**/

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
    debugUDP();
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
