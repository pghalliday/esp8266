void setup() {
  Serial.begin(9600);
  config_setup(D2, 5000);
}

void loop() {
  config_loop();
}
