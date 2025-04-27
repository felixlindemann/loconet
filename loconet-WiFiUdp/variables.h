
 
std::map<int, int> signals; // signal 1/2 means, signal 1 has 2 LED  
std::map<int, int> signalUbound; // 3/12 means signal 3 starts on Port 12
std::map<int, int> outPut; // 4 / 1 means out turn 4 on

const int maxLED = 64;
boolean POWERON;

int LEDCOUNT = 0;
uint32_t waitUnti =0;
uint32_t LoopTimer =0;
boolean  Data_Updated =false;
uint32_t EPROM_Write_Delay ; 
const uint32_t Ten_Sec = 10000;
uint32_t MSTimer =0;

// LED Blinkrate
uint32_t blinkTime = 1000;



uint8_t SV[maxLED];
uint8_t CV[maxLED];
// 0 = aus
// 1 = an
// 2 = blink
int LEDSTATE[maxLED];
boolean LEDRequiresChange[maxLED];

// Switch LED Nextime
uint32_t LEDWaitUntil[maxLED];

void WriteSV2EPROM() {

  for (int i = 0; i <= maxLED-1; i++) {
    EEPROM.write(i+256*0, SV[i]); 
    EEPROM.write(i+256*1, LEDSTATE[i]); 
    EEPROM.write(i+256*2, CV[i]); 
  }
}

void EPROM2SV() {
  for (int i = 0; i <= maxLED-1; i++) {
    SV[i ] = EEPROM.read(i+256*0); 
    LEDSTATE[i] = EEPROM.read(i+256*1); 
    CV[i] = EEPROM.read(i+256*2); 
  }
}

void eePromSetup(){

  Data_Updated = false;
  EEPROM.begin(512);

  if ((EEPROM.read(255) == 0xFF) && (EEPROM.read(256) == 0xFF)) {  //eeprom empty, first run. Can also set via CV[8]=8
    Serial.println(" ******* EPROM EMPTY....Setting Default EEPROM values *****");
    
    WriteSV2EPROM();
    Data_Updated = true;
    EPROM_Write_Delay = millis() + Ten_Sec;
    //EEPROM.commit();
    //delay(100);
  }  //if
}

void eeprom_Loop(){

 if ((millis() >= EPROM_Write_Delay) && (Data_Updated)) {  // commit EEPROM only when needed..
    
#if _SERIAL_DEBUG
    Serial.println("Commiting EEPROM");
#endif

  //  SetMyPorts();  //make any port direction changes..

    // FullBoardAddress = AddrFull(SV[2], SV[1]);  // +++++++++  Set up things that may have been changed...+++++++++
    // MyLocoLAddr = CV[18] + ((CV[17] & 0x3F) * 256);
    // MyLocoAddr = CV[1];  ///
    // EEPROM.commit();
  // Data_Updated = false;
    //ESP.reset() ;
  }   


} 


