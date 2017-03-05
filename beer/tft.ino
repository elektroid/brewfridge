#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library

#define X_AXIS_YPOS 100
#define Y_AXIS_XPOS 28
#define POINTS 118
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
  digitalWrite(TFTSWITCH, HIGH);
  digitalWrite(BACKLIGHT, HIGH);

  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  Serial.println("Initialized");
  tft.setRotation(180);
  tft.fillScreen(ST7735_BLACK);
  drawFrame();
  lowPart();
}

unsigned long lastDraw = millis();

void display() {
  if ((millis() - lastDraw) < 50) {
    return;
  }
  miscData();
  runningTimeAndState();

  if ((millis() - lastDraw) < 60000) {
    return;
  }
  lowPart();
  drawTemp();
  lastDraw = millis();
}


float previousCurrentTemp = -1;
void miscData() {
  if (previousCurrentTemp != getTargetTemp()) {
    tft.fillRect(0, 0, 128, 30, ST7735_BLACK  );
    tft.setTextSize(1);
    tft.setCursor(20, 8);
    tft.print("target: ");
    tft.print(getTargetTemp());
  }
  previousCurrentTemp = getTargetTemp();
}

void drawFrame() {

  // x axis
  tft.drawLine(Y_AXIS_XPOS, X_AXIS_YPOS, Y_AXIS_XPOS + POINTS + 1, X_AXIS_YPOS, ST7735_BLUE);

  // temp marks
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  for (int i = 1; i < 4; i++) {
    // draw a small horizontal line to mark 10,20,30 degrees c
    tft.drawLine(Y_AXIS_XPOS - TEMP_DASH_WIDTH, X_AXIS_YPOS - i * 20, Y_AXIS_XPOS - 1, X_AXIS_YPOS - i * 20, ST7735_RED);

    tft.setCursor(10, X_AXIS_YPOS - i * (25 - i));
    tft.print(10 * i);
  }

  for (int i = 1 ; i < 8; i++) {
    tft.drawLine(Y_AXIS_XPOS - TEMP_DASH_WIDTH / 2, X_AXIS_YPOS - i * 10, Y_AXIS_XPOS - 1, X_AXIS_YPOS - i * 10, ST7735_RED);
  }

  // y axis
  tft.drawLine(Y_AXIS_XPOS, X_AXIS_YPOS, Y_AXIS_XPOS, 28, ST7735_BLUE);
}

void lowPart() {

  tft.fillRect(0, 110, 128, 50, ST7735_BLACK  );
  // External Temperature section
  tft.setCursor(8, 110);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.println("Temp:");
  tft.setTextSize(2);


  tft.print(getCurrentExternalTemp()); tft.println("C");
  tft.setTextSize(1);
  tft.setTextColor(ST7735_YELLOW);
  tft.setCursor(2, 140);
  tft.print("max:"); tft.println(maxTemp);
  tft.setCursor(2, 148);
  tft.print("min:"); tft.print(minTemp);

  // Internal Temp section
  tft.setCursor(50, 110);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_MAGENTA);
  tft.println("Internal:");
  tft.setTextSize(2);

  tft.setCursor(52, 120);
  tft.print(getCurrentInternalTemp()); tft.println("C");
}

void runningTimeAndState() {
  tft.fillRect(64, 138, 128, 138, ST7735_BLACK  );
  //misc
  tft.setTextSize(1);
  tft.setCursor(64, 140);
  tft.setTextColor(ST7735_YELLOW);
  float elapsed = runningTime() / 60000.0;
  tft.print("run:"); tft.println(elapsed);

  tft.setCursor(64, 148);
  //tft.print("state:");
  if (fridgeRunning) {
    tft.setTextColor(ST7735_GREEN);
    tft.print("running");
  } else if (fridgeWait) {
    tft.setTextColor(ST7735_YELLOW);
    tft.print("waiting");
  } else {
    tft.setTextColor(0x5F55);
    tft.print("ok");
  }
}

uint8_t xoffset = 0;

void drawTemp() {

  xoffset++;
  if (xoffset >= POINTS) {
    tft.fillScreen(ST7735_BLACK);
    drawFrame();
    xoffset = 0;
  }

  tft.drawFastVLine(Y_AXIS_XPOS + xoffset + 1, X_AXIS_YPOS - (int)(2 * getCurrentInternalTemp() + 0.5), (int)(2 * getCurrentInternalTemp() + 0.5), ST7735_MAGENTA);
  tft.drawRect(Y_AXIS_XPOS + xoffset + 1, X_AXIS_YPOS - (int)getCurrentExternalTemp() * 2 , 1, 1, ST7735_WHITE);
}

void printBattery() {
  tft.setCursor(24, 10);
  tft.setTextColor(0x5555);
  tft.print("Battery Vcc:");
  tft.println(readVcc());
}



