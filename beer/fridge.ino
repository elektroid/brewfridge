#define FRIDGE_PIN A0
#define FRIDGE_DELAY 600000 //10 minutes in ms
#define OVERSHOOT 0.5
// init lastrun with start time, in case of intempestive restart we make sure we let the compressor rest
unsigned long lastrun = millis();
bool fridgeRunning = false;


void setup_fridge() {

  pinMode(FRIDGE_PIN, OUTPUT);
  digitalWrite(FRIDGE_PIN, LOW);
}

void handle_fridge(float targetTemp, float currentTemp) {

  // first do we need action ?
  if (targetTemp - currentTemp > OVERSHOOT) { // we are too hot

    if (!fridgeRunning) {
      // can we start now without stressing the compressor ?
      if (millis() - lastrun > FRIDGE_DELAY) {
        fridgeRunning = true;
      }
    }

  } else {
    // we are cold enough
    fridgeRunning = false;
  }
  if (fridgeRunning) lastrun = millis();
  digitalWrite(FRIDGE_PIN, fridgeRunning);
}

