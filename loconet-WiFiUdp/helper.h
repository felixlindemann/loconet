
void dump_byte(uint8_t buffer) {
 
       Serial.print(buffer < 0x10 ? " 0" : " ");
       Serial.print(buffer, HEX);
      // Serial.print("H ");

}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
      dump_byte(buffer[i]); 
    }
}

uint8_t CommandData (uint8_t *RECMsg) {
   uint8_t Data;
   bitWrite (Data, 1, bitRead(RECMsg[2],5));
   bitWrite (Data, 0, bitRead(RECMsg[2],4)); 
  
 return (Data);
}

uint16_t AddrFull(uint8_t HI, uint8_t LO) {
  uint16_t FullAddress;
  FullAddress = 128 * (HI & 0x0F) + LO;
  return (FullAddress);
}

uint16_t CommandFor (uint8_t *RECMsg) {
   uint16_t Address;
   Address= 255;
   switch (RECMsg[0]){       
    case 0xA0: // OPC_LOCO_SND)
       Address= ( RECMsg[1]); 
        break;      
    case 0xA1: // OPC_LOCO_SND)
       Address= ( RECMsg[1]); 
        break;    
    case 0xA2: // OPC_LOCO_SND)
       Address= ( RECMsg[1]); 
        break; 
    case 0xB0://OPC_SW_REQ) 
       Address= ((128*(RECMsg[2]&0x0F)+RECMsg[1])+1); 
    break;
    case 0xBB: //OPC_RQ_SL_DATA)
       Address= ( RECMsg[1]); 
        break; 
     case 0xBF: // OPC_LOCO_ADR) 
        Address= ( (128*(RECMsg[1])+RECMsg[2])); 
       break;
    case 0xE4: // Signale E4 09 00 1F 11 00 11 00 0D
        Address= ((128*(RECMsg[2] )+RECMsg[3])); 
       break;
    case 0xE5: // OPC_PEER_XFER)
        Address= ((128*(RECMsg[11]&0x0F)+RECMsg[3])); 
       break;
    case 0xEF: // OPC_WR_SL_DATA)
        Address= ( RECMsg[6]+(128*RECMsg[5])); 
        break;
    default:
        Address=255;
    break;
   }
 return (Address);
}
