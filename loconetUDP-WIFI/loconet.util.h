#include "OPC.h"


uint8_t lnCalcCheckSumm(uint8_t *cMessage, uint8_t cMesLen) {
  unsigned char ucIdx = 0;
  char cLnCheckSum = 0;

  for (ucIdx = 0; ucIdx < cMesLen - 1; ucIdx++)  //check summ is the last byte of the message
  {
    cLnCheckSum ^= cMessage[ucIdx];
  }

  return (~cLnCheckSum);
}

bool isValidLocoNetChecksum(const uint8_t *packet) {
  uint8_t length = sizeof(packet) / sizeof(packet[0]);


  uint8_t sum = 0xFF;
  for (uint8_t i = 0; i < length - 1; i++) {
    sum ^= packet[i];
  }

  return sum == packet[length - 1];
}
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
}
void dump_byte_array(byte *buffer) {
  byte bufferSize = sizeof(buffer) / sizeof(buffer[0]);
  dump_byte_array(buffer, bufferSize);
}
void dump_byte(uint8_t buffer) {

  Serial.print(buffer < 0x10 ? " 0" : " ");
  Serial.print(buffer, HEX);
}


uint16_t AddrFull(uint8_t HI, uint8_t LO) {
  uint16_t FullAddress;
  FullAddress = 128 * (HI & 0x0F) + LO;
  return (FullAddress);
}




uint16_t CommandFor(uint8_t *msg) {
  uint16_t Address;
  Address = 255;
  switch (msg[0]) {
    case 0xA0:  // OPC_LOCO_SND)
      Address = (msg[1]);
      break;
    case 0xA1:  // OPC_LOCO_SND)
      Address = (msg[1]);
      break;
    case 0xA2:  // OPC_LOCO_SND)
      Address = (msg[1]);
      break;
    case 0xB0:  //OPC_SW_REQ)
      Address = ((128 * (msg[2] & 0x0F) + msg[1]) + 1);
      break;
    case 0xBB:  //OPC_RQ_SL_DATA)
      Address = (msg[1]);
      break;
    case 0xBF:  // OPC_LOCO_ADR)
      Address = ((128 * (msg[1]) + msg[2]));
      break;
    case 0xE5:  // OPC_PEER_XFER)
      Address = ((128 * (msg[11] & 0x0F) + msg[3]));
      break;
    case 0xE4:  // OPC_PEER_XFER)
      Address = ((128 * (msg[2] & 0x0F) + msg[3]))+1;
      break;
    case 0xEF:  // OPC_WR_SL_DATA)
      Address = (msg[6] + (128 * msg[5]));
      break;
    default:
      Address = 255;
      break;
  }
  return (Address);
}

uint8_t CommandData(uint8_t *msg, uint8_t len) {
  uint8_t Data;
  bitWrite(Data, 1, bitRead(msg[2], 5));
  bitWrite(Data, 0, bitRead(msg[2], 4));

  return (Data);
}
uint8_t CommandOpCode(uint8_t *msg, uint8_t len) {
  uint8_t Data;
  Data = msg[0];
  return (Data);
}
void unkownCommand(uint8_t *data, uint8_t length) {
  Serial.print("Unbekanntes LocoNet-Paket: ");
  dump_byte_array(data, length);
}

void SwitchRequest(uint8_t *data) {
  uint16_t address = CommandFor(data);
  uint8_t cmd = CommandData(data, 4);
  uint8_t sw1 = data[1];
  uint8_t sw2 = data[2];

  // int address = ((sw2 & 0x0F) << 7) + (sw1 & 0x7F) + 1;
  bool thrown = sw1 & 0x10;
  bool active = sw1 & 0x20;
  Serial.print("LocoNet: OPC_SW_REQ empfangen → Adresse: ");
  Serial.print(address);
  Serial.print(", Zustand: ");
  Serial.print(thrown ? "THROWN" : "CLOSED");
  Serial.print(", Aktiv: ");
  Serial.println(active ? "JA" : "NEIN");
}

void Len4commands(uint8_t *data) {
  uint16_t CVRequest;
  uint8_t CV_Data;

  switch (data[0]) {
    case OPC_SW_REQ:
      SwitchRequest(data);
      break;
    default:
      unkownCommand(data, 4);
      break;
  }
}

void switchSignal(uint8_t *data) {
  if (data[0] == 0xE4 && data[1] == 0x09) {

    // E4 09 00 1F 11 00 11 00 0D
  // E4 = LocoNet OPC_SW_REQ (Switch Request)
  // 09 = Länge des Pakets
  // 00 1F = Adressierung / Portnummer
  // 11 = Steuerdaten
  // 00 11 Daten
  // 00 0D Prüfsumme

    uint16_t address = CommandFor(data); 
    int port = 0;
    uint16_t led = data[5]*128 + data[6];

    uint16_t MaxLED = led >> 8;

    Serial.print("LocoNet: OPC_SW_SIGNAL empfangen → Adresse: ");
     Serial.printf("\n adresse: %d Port: %d Value: %d\n", address,port, led);
Serial.print("Bitmaske Vorhanden: ");
Serial.println(MaxLED, BIN);
Serial.print("Bitmaske Aktiv.   : ");
Serial.println(led - (MaxLED << 8), BIN);

Serial.print( "Turning off... ");
    for (int i = 0; i < 8; i++) {
      if ((MaxLED & (1 << i)) && !(led & (1 << i))) {
        // MCPSetLED(address + i, false);
        Serial.print( address + i );
        Serial.print( " - " );
      }
    }
     delay(200);
Serial.println();
Serial.print( "Turning on... ");
    for (int i = 0; i < 8; i++) {
      if ((MaxLED & (1 << i)) && (led & (1 << i))) {
        // MCPSetLED(address + i, true);
         Serial.print( address + i );
        Serial.print( " - " );
      }
    }


  }
}

void Len9commandsE4(uint8_t *data) {
  uint16_t CVRequest;
  uint8_t CV_Data;
  if (data[0] == 0xE4) {
    switch (data[1]) {
      case 0x09:
        switchSignal(data);
        break;
      default:
        unkownCommand(data, 9);
        break;
    }}
    else {
    }
  
}
void Len9commands(uint8_t *data) {
  uint16_t CVRequest;
  uint8_t CV_Data;

  switch (data[0]) {
    case 0xE4:
      Len9commandsE4(data);
      break;
    default:
      unkownCommand(data, 9);
      break;
  }
}


// 🔍 Dekodiert LocoNet OPC_SW_REQ
void decodeLocoNetPacket(uint8_t *data, uint8_t length) {

  switch (length) {
    case 4:
      Len4commands(data);
      break;

    case 9:
      Len9commands(data);
      break;

    default:
      unkownCommand(data, length);

      break;
  }
}
