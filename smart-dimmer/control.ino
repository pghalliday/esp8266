ICACHE_RAM_ATTR void control_AChange();
ICACHE_RAM_ATTR void control_BChange();

int control_pinA;
int control_pinB;

volatile unsigned int control_aRisingCount = 0;
volatile unsigned int control_aFallingCount = 0;
volatile unsigned int control_bRisingCount = 0;
volatile unsigned int control_bFallingCount = 0;

void control_setup(int pinA, int pinB) {
  control_pinA = pinA;
  control_pinB = pinB;
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinA), control_AChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB), control_BChange, CHANGE);
}

void control_loop() {
  Serial.print("control_aRisingCount: ");
  Serial.print(control_aRisingCount);
  Serial.print(", control_bFallingCount: ");
  Serial.print(control_aFallingCount);
  Serial.print(", control_bRisingCount: ");
  Serial.print(control_aRisingCount);
  Serial.print(", control_bFallingCount: ");
  Serial.println(control_bFallingCount);
}

void control_AChange() {
  if (digitalRead(control_pinA) == HIGH) {
    control_aRisingCount++;
  } else {
    control_aFallingCount++;
  }
}

void control_BChange() {
  if (digitalRead(control_pinB) == HIGH) {
    control_bRisingCount++;
  } else {
    control_bFallingCount++;
  }
}
