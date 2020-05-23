#define READ_PIN 2
#define LOW_STATE 0
#define HIGH_STATE 1

unsigned long now = micros();
unsigned long lastStateChangedMicros = micros();
int state = HIGH_STATE;
int delimiter = 0;

// 初期化
void setup(){
  Serial.begin(57600);
  pinMode(READ_PIN, INPUT);
  Serial.print("\n");
}

void waitLow() {
  while (digitalRead(READ_PIN)==LOW) {
    ;
  }
}

int waitHigh() {
  unsigned long start = micros();
  while (digitalRead(READ_PIN)==HIGH) {
    if (micros() - start > 5000000) {
      return 1;
    }
  }
  return 0;
}


void loop() {
    if (state == LOW_STATE) {
      waitLow();
    } else {
      int ret = waitHigh();
      if (ret == 1) {
        Serial.print("\n");
        return;
      }
    }
    now = micros();
    if (delimiter == 1) {
      Serial.print(",");
    }
    Serial.print((now - lastStateChangedMicros) / 10, DEC);
    delimiter  = 1;
    lastStateChangedMicros = now;
    if (state == HIGH_STATE) {
      state = LOW_STATE;
    } else {
      state = HIGH_STATE;
    }
}
