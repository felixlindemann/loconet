
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // or 4 or whatever your display uses
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void i2cSetup (){

  // I2C (Custom pins, if on ESP32)
  Wire.begin(15, 13);  // SDA, SCL

  // Init display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 not found"));
    while (1);
  }
  display.clearDisplay();
}

void i2cLoop(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Print IP
  display.setCursor(0, 0);
  display.print("IP: ");
  display.println(WiFi.localIP());

  // Print LED status
  for (int i = 0; i < 16; i++) {
    bool state1 = mcp1.digitalRead(i);
    bool state2 = mcp2.digitalRead(i);
    
    display.setCursor((i % 8) * 15, 16 + (i / 8) * 8);
    display.print(state1 ? "1" : "0");

    display.setCursor((i % 8) * 15, 40 + (i / 8) * 8);
    display.print(state2 ? "1" : "0");
  }

  display.display();

}