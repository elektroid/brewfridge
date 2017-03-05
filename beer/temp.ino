#define ONE_WIRE_BUS 7

#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2     // what digital pin we're connected to
DHT dht(DHTPIN, DHTTYPE);


// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

int currentExternalTemp;
float currentInternalTemp = 0.0;


float getCurrentInternalTemp() {
  return currentInternalTemp;
}

int getCurrentExternalTemp() {
  return currentExternalTemp;
}

bool setup_temp() {

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  if (!sensors.getAddress(insideThermometer, 0)) {
    Serial.println("Unable to find address for Device 0");
    return false;
  }
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC);
  Serial.println();
  return true;

  dht.begin();

  read_temps();
  read_temps();
}

void read_temps() {
  external_hum_temp();
  read_internal_temp(insideThermometer);
}

float maxTemp = -1;
float minTemp = 99;

// function to print the temperature for a device
void read_internal_temp(DeviceAddress deviceAddress)
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print("Temperature for the device 1 (index 0) is: ");
  currentInternalTemp = sensors.getTempCByIndex(0);
  Serial.println(currentInternalTemp);

  if (getCurrentInternalTemp() > maxTemp) {
    maxTemp = getCurrentInternalTemp();
  }
  if (getCurrentInternalTemp() < minTemp) {
    minTemp = getCurrentInternalTemp();
  }
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

int external_hum_temp() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

  //ht[0] = (int)dht.readHumidity();
  // Read temperature as Celsius (the default)
  currentExternalTemp = (int)dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if ( isnan(currentExternalTemp) ) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  }

  Serial.print("ext Temperature: ");
  Serial.print(getCurrentExternalTemp());
  //  Serial.print(" *C ");
  //  Serial.println("");

  return 1;
}


