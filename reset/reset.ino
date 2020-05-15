// just checking button presses with INPUT_PULLUP

void setup() {
  Serial.begin(9600);
  pinMode(D2, INPUT_PULLUP);
}

void loop() {
  int state = digitalRead(D2);
  Serial.println(state);
  delay(1000);
}
