#define FRIDGE_PIN A4
#define FRIDGE_DELAY 900000 //15 minutes in ms
#define FRIDGE_MINIMUM_RUN 900000 //15 minutes in ms

// init stateChange with start time, in case of intempestive restart we make sure we let the compressor rest
unsigned long stateChange = millis();
bool fridgeRunning = false;
bool fridgeWait = false;

#define NORMAL 1
#define FULL_ON 2
#define FULL_OFF 3

float targetTemp = 20.0;
byte specialState = NORMAL;

void setup_fridge(float target) {

  targetTemp = target;
  pinMode(FRIDGE_PIN, OUTPUT);
  digitalWrite(FRIDGE_PIN, LOW);
}

float getTargetTemp() {
  return targetTemp;
}

void tempUp() {
  targetTemp += 0.5;
}

void tempDown() {
  targetTemp -= 0.5;
}

void off() {
  specialState = FULL_OFF;
  // not a real mode for now
  targetTemp = 99;
}

void on() {
  specialState = FULL_ON;
  // not a real mode for now
  targetTemp = -99;
}


long runningTime() {
  if (fridgeRunning) {
    return millis() - stateChange;
  }
  return stateChange - millis();
}

void handle_fridge(float currentTemp) {
  fridgeWait = false;
  Serial.print("Fridge is ");
  Serial.println(fridgeRunning);
  // first do we need action ?

  if ( currentTemp  > targetTemp) { // we are too hot
    Serial.print("Temperature is too hot, target is");
    Serial.println(targetTemp);
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
    // run at leat 5 minutes
    if (millis() - stateChange > FRIDGE_MINIMUM_RUN) {
      if (fridgeRunning) {
        stateChange = millis();
      }
      fridgeRunning = false;
    }
  }

  digitalWrite(FRIDGE_PIN, fridgeRunning);
}

