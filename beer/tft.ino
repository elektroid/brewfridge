
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library

#define X_AXIS_YPOS 100
#define Y_AXIS_XPOS 32
#define TEMP_DASH_WIDTH 5

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10  //CS - CE
#define TFT_RST    8  // RESET you can also connect this to the Arduino reset
// in which case, set this #define pin to 0!
#define TFT_DC     9   //A0 - DC - RS - D/C
#define BACKLIGHT 3
#define TFTSWITCH 6


// Option 2: use any pins but a little slower!
#define TFT_SCLK 13   // set these to be whatever pins you like! SPI/SCK
#define TFT_MOSI 11   // set these to be whatever pins you like!   SPI - SDA - DIN
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);


void setup_tft() {
  pinMode(TFTSWITCH, OUTPUT);
  pinMode(BACKLIGHT, OUTPUT);
  digitalWrite(TFTSWITCH, LOW);
  digitalWrite(BACKLIGHT, LOW);
}


void display() {

  // Use this initializer if you're using a 1.8" TFT
  digitalWrite(TFTSWITCH, HIGH);

  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  Serial.println("Initialized");
  tft.fillScreen(ST7735_BLACK);
  digitalWrite(BACKLIGHT, HIGH);

  drawFrame();
  drawTemp();

}



float maxTemp = -1;
float minTemp = 99;
float maxHum = -1;
float minHum = 99;
void drawFrame() {

  // check min/max
  if (ht[0] > maxHum) {
    maxHum = ht[0];
  }
  if (ht[0] < minHum) {
    minHum = ht[0];
  }
  if (ht[1] > maxTemp) {
    maxTemp = ht[1];
  }
  if (ht[1] < minTemp) {
    minTemp = ht[1];
  }

  // x axis
  tft.drawLine(Y_AXIS_XPOS, X_AXIS_YPOS, tft.width() - 1, X_AXIS_YPOS, ST7735_BLUE);

  // temp marks
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  for (int i = 1; i < 4; i++) {
    // draw a small horizontal line to mark 10,20,30 degrees c
    tft.drawLine(Y_AXIS_XPOS - TEMP_DASH_WIDTH, X_AXIS_YPOS - i * 20, Y_AXIS_XPOS - 1, X_AXIS_YPOS - i * 20, ST7735_RED);
    tft.setCursor(10, X_AXIS_YPOS - i * 25);
    tft.print(10 * i);
  }

  // y axis
  tft.drawLine(Y_AXIS_XPOS, X_AXIS_YPOS, Y_AXIS_XPOS, 32, ST7735_BLUE);


  // Temperature section
  tft.setCursor(8, 110);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_GREEN);
  tft.println("Temp:");
  tft.setTextSize(2);


  tft.print(ht[1]); tft.println("C");
  tft.setTextSize(1);
  tft.setTextColor(ST7735_YELLOW);
  tft.setCursor(2, 140);
  tft.print("max:"); tft.println(maxTemp);
  tft.setCursor(2, 148);
  tft.print("min:"); tft.print(minTemp);

  // Humidity section
  tft.setCursor(72, 110);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_GREEN);
  tft.println("Humidity:");
  tft.setTextSize(2);

  tft.setCursor(72, 120);
  tft.print(ht[0]); tft.println("%");
  tft.setTextSize(1);
  tft.setCursor(74, 140);
  tft.setTextColor(ST7735_YELLOW);
  tft.print("max:"); tft.println(maxHum);
  tft.setCursor(74, 148);
  tft.print("min:"); tft.print(minHum);
}

uint8_t xoffset = 0;
uint8_t temps[96];
uint8_t itemps[96];
void drawTemp() {
  if (xoffset++ >= 96) {
    for (int i = 0; i < 96; i++) {
      temps[i] = 0;
    }
    xoffset = 0;
  }
  temps[xoffset] = ht[1];
  itemps[xoffset] = (int)(getCurrentInternalTemp()+0.5);


  for (int i = 0; i <= xoffset; i++) {
    //     tft.drawPixel(Y_AXIS_XPOS+i,X_AXIS_YPOS-temps[i]*2, ST7735_MAGENTA);
    tft.drawLine(Y_AXIS_XPOS + i, X_AXIS_YPOS - 1, Y_AXIS_XPOS + i, X_AXIS_YPOS - (int)temps[i] * 2, ST7735_WHITE);
//    delay(10);
    tft.drawLine(Y_AXIS_XPOS + i, X_AXIS_YPOS - 1, Y_AXIS_XPOS + i, X_AXIS_YPOS - (int)temps[i] * 2, ST7735_MAGENTA);

  }


}

void printBattery() {
  tft.setCursor(24, 10);
  tft.setTextColor(0x5555);
  tft.print("Battery Vcc:");
  tft.println(readVcc());

}



