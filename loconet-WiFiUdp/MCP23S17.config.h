#define csPinMcp1 5
#define csPinMcp2 32

Adafruit_MCP23X17 mcp1;  // für CS Pin 10
Adafruit_MCP23X17 mcp2;  // für CS Pin 9

void SetupMCP23S17(){
  if (!mcp1.begin_SPI(csPinMcp1)) {
    Serial.println("Fehler beim Initialisieren von MCP1");
    while (1);
  }

  if (!mcp2.begin_SPI(csPinMcp2)) {
    Serial.println("Fehler beim Initialisieren von MCP2");
    while (1);
  }

  // Beispiel: Setze alle Pins von MCP1 als Output
  for (uint8_t i = 0; i < 16; i++) {
    mcp1.pinMode(i, OUTPUT);
    mcp2.pinMode(i, OUTPUT);
  }

  // Schalte alle Pins auf HIGH
  for (uint8_t i = 0; i < 16; i++) {
    mcp1.digitalWrite(i, HIGH);
    mcp2.digitalWrite(i, LOW);
  }
}

void MCPSetLED(uint8_t i, bool value ){


  if(i >=16 ){
      mcp2.digitalWrite(i-16, value? HIGH: LOW);
  }else{
      mcp1.digitalWrite(i, value? HIGH: LOW);
  }

}
