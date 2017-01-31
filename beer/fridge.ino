#define FRIDGE_PIN A0
#define FRIDGE_DELAY 600000 //10 minutes in ms
#define OVERSHOOT 0.5
// init stateChange with start time, in case of intempestive restart we make sure we let the compressor rest
unsigned long stateChange = millis();
bool fridgeRunning = false;
bool fridgeWait = false;


void setup_fridge() {

  pinMode(FRIDGE_PIN, OUTPUT);
  digitalWrite(FRIDGE_PIN, LOW);
}

long runningTime() {
  if (fridgeRunning) {
    return millis() - stateChange;
  }
  return stateChange - millis();
}

void handle_fridge(float targetTemp, float currentTemp) {
  fridgeWait = false;
  Serial.print("Fridge is ");
  Serial.println(fridgeRunning);
  // first do we need action ?

  if ( currentTemp - OVERSHOOT > targetTemp) { // we are too hot
    Serial.println("Temperature is too hot");
    if (!fridgeRunning) {
      // can we start now without stressing the compressor ?
      if (millis() - stateChange > FRIDGE_DELAY) {
        fridgeRunning = true;
        stateChange = millis();
      } else {
        fridgeWait = true;
      }
    }

  } else {
    // we are cold enough
    if (fridgeRunning) {
      stateChange = millis();
    }
    fridgeRunning = false;
  }

  digitalWrite(FRIDGE_PIN, fridgeRunning);
}

