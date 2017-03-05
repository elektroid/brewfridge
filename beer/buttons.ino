
int lastA0State = LOW;   // the previous reading from the input pin
int lastA1State = LOW;
int lastA2State = LOW;
int lastA3State = LOW;


void setup_buttons() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

}

int handle_buttons() {

  // If the switch changed, due to noise or pressing:
  if (digitalRead(A0) != lastA0State || digitalRead(A1) != lastA1State || digitalRead(A2) != lastA2State || digitalRead(A3) != lastA3State)  {
    // active debouncing
    delay(50);
    lastA0State = digitalRead(A0);
    lastA1State = digitalRead(A1);
    lastA2State = digitalRead(A2);
    lastA3State = digitalRead(A3);

    Serial.print("button a0: ");
    Serial.println(lastA0State);
    Serial.print("button a1: ");
    Serial.println(lastA1State);
    Serial.print("button a2: ");
    Serial.println(lastA2State);
    Serial.print("button a3: ");
    Serial.println(lastA3State);

    if (lastA0State) {
      tempUp();
      return 1;
    }
    if (lastA1State) {
      tempDown();
      return 1;
    }
    if (lastA2State) {
      off();
      return 1;
    }
    if (lastA3State) {
      on();
      return 1;
    }
  }
  return 0;

}

