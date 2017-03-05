#include <SPI.h>
#include <SD.h>

#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define INIT_TARGET_TEMPERATURE 18.0

void setup(void) {
  Serial.begin(115200);
  //Let's make it more random
  randomSeed(analogRead(0));

  setup_fridge(INIT_TARGET_TEMPERATURE);
  setup_temp();
  setup_tft();
  setup_buttons();
  // setup_log();
}

void loop() {
  read_temps();
  handle_fridge( getCurrentInternalTemp());
  handle_buttons();
  display();
}







