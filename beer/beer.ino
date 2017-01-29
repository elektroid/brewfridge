#include <SPI.h>
#include <SD.h>

#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>


float targetTemp = 18.0;

void setup(void) {
  Serial.begin(9600);
  //Let's make it more random
  randomSeed(analogRead(0));

  setup_fridge();
  setup_temp();
  setup_tft();
  setup_log();

}



void loop() {

  read_temps();
 // handle_fridge( targetTemp,  getCurrentInternalTemp());

  display();
  delay(100);
}


















