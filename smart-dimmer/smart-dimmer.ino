void setup() {
  Serial.begin(9600);
  control_setup(D5, D6);
  config_setup(D2, 5000);
}

void loop() {
  control_loop();
  config_loop();
}
