#include <LowPower.h>


#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2     // what digital pin we're connected to
DHT dht(DHTPIN, DHTTYPE);

#define BACKLIGHT 3
#define TFTSWITCH 6

#define ONE_WIRE_BUS 7
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10  //CS - CE
#define TFT_RST    8  // RESET you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     9   //A0 - DC - RS - D/C
#define SD_CS    4  // Chip select line for SD card

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
#define TFT_SCLK 13   // set these to be whatever pins you like! SPI/SCK
#define TFT_MOSI 11   // set these to be whatever pins you like!   SPI - SDA - DIN
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);



File root;
int ht[2];


void setup(void) {
  Serial.begin(9600);
  //Let's make it more random
  randomSeed(analogRead(0));
  pinMode(TFTSWITCH, OUTPUT);
  pinMode(BACKLIGHT, OUTPUT);
  digitalWrite(TFTSWITCH, LOW);
  digitalWrite(BACKLIGHT, LOW);   
  initInternalTempSensor();
  
 // dht.begin();
//  initSd();
//  root = SD.open("/");
//  printDirectory(root, 0);
  //gfx();


 
}

void initInternalTempSensor(){

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
   if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);
 
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();
}


void display(){
 
  // Use this initializer if you're using a 1.8" TFT
  digitalWrite(TFTSWITCH, HIGH);
    
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  Serial.println("Initialized"); 
  tft.fillScreen(ST7735_BLACK);
  digitalWrite(BACKLIGHT, HIGH); 

  drawFrame();
  drawTemp();
  
}

void loop() {
   
  display();
  delay(2000);

  // let's sleep 15 minutes
  /*
  for (int i=0;i<15*60/8;i++){
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
  }
  */
  
}



#define X_AXIS_YPOS 100
#define Y_AXIS_XPOS 32
#define TEMP_DASH_WIDTH 5
float maxTemp=-1;
float minTemp=99;
float maxHum=-1;
float minHum=99;
void drawFrame(){
    if (!hum_temp()){
      Serial.print("Failed to read");
      return;
    }
    

    // check min/max
    if (ht[0]>maxHum){
        maxHum=ht[0];
    }
    if (ht[0]<minHum){
        minHum=ht[0];
    }
    if (ht[1]>maxTemp){
        maxTemp=ht[1];
    }
    if (ht[1]<minTemp){
        minTemp=ht[1];
    }
  
    // x axis
    tft.drawLine(Y_AXIS_XPOS, X_AXIS_YPOS, tft.width()-1, X_AXIS_YPOS, ST7735_BLUE);
    
    // temp marks
    tft.setTextColor(ST7735_RED);
    tft.setTextSize(1);
    for (int i=1;i<4;i++){
       // draw a small horizontal line to mark 10,20,30 degrees c
       tft.drawLine(Y_AXIS_XPOS-TEMP_DASH_WIDTH, X_AXIS_YPOS-i*20, Y_AXIS_XPOS-1, X_AXIS_YPOS-i*20, ST7735_RED);
       tft.setCursor(10,X_AXIS_YPOS-i*25);
       tft.print(10*i);
    }
    
    // y axis
    tft.drawLine(Y_AXIS_XPOS, X_AXIS_YPOS, Y_AXIS_XPOS, 32, ST7735_BLUE);
    
   
    
    // Temperature section
    tft.setCursor(8,110);
    tft.setTextSize(1);
    tft.setTextColor(ST7735_GREEN);
    tft.println("Temp:");
    tft.setTextSize(2);


    tft.print(ht[1]); tft.println("C");
    tft.setTextSize(1);
    tft.setTextColor(ST7735_YELLOW);
    tft.setCursor(2,140);
    tft.print("max:");tft.println(maxTemp);
    tft.setCursor(2,148);
    tft.print("min:");tft.print(minTemp);
    
    // Humidity section
    tft.setCursor(72,110);
    tft.setTextSize(1);
    tft.setTextColor(ST7735_GREEN);
    tft.println("Humidity:");
    tft.setTextSize(2);

    tft.setCursor(72,120);
    tft.print(ht[0]); tft.println("%");
    tft.setTextSize(1);
    tft.setCursor(74,140);
    tft.setTextColor(ST7735_YELLOW);
    tft.print("max:");tft.println(maxHum);
    tft.setCursor(74,148);
    tft.print("min:");tft.print(minHum); 
}

uint8_t xoffset=0;
uint8_t temps[96];
uint8_t itemps[96];
void drawTemp(){
  if (xoffset++>=96){
      for (int i=0;i<96;i++){
         temps[i]=0;
      }
     xoffset=0; 
  }
  temps[xoffset]=ht[1];


  for (int i=0;i<=xoffset;i++){
//     tft.drawPixel(Y_AXIS_XPOS+i,X_AXIS_YPOS-temps[i]*2, ST7735_MAGENTA);
     tft.drawLine(Y_AXIS_XPOS+i, X_AXIS_YPOS-1, Y_AXIS_XPOS+i,X_AXIS_YPOS-(int)temps[i]*2, ST7735_MAGENTA);
     delay(10);
     tft.drawLine(Y_AXIS_XPOS+i, X_AXIS_YPOS-1, Y_AXIS_XPOS+i,X_AXIS_YPOS-(int)temps[i]*2, ST7735_MAGENTA);
     
  }
  tft.setCursor(24,10);
  tft.setTextColor(0x5555);
  tft.print("Battery Vcc:");
  tft.println(readVcc());
     
  
}

int hum_temp(){
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  
  ht[0] = (int)dht.readHumidity();
  // Read temperature as Celsius (the default)
  ht[1] = (int)dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(ht[0]) || isnan(ht[1]) ) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  }

  Serial.print("Humidity: ");
  Serial.print(ht[0]);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(ht[1]);
  Serial.print(" *C ");
  
  return 1;
}




// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{

  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
 
}



void initSd(){
  
    Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(SD_CS)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
}

void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}


void logTemperature(String dataString){
    // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
}


long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}



