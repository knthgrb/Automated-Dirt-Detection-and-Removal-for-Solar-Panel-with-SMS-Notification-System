const int limitSwitchPin = 6;  // Digital pin connected to the limit switch

void setup() {
  pinMode(limitSwitchPin, INPUT);
}

void loop() {
  // Read the state of the limit switch
  int limitSwitchState = digitalRead(limitSwitchPin);

  Serial.println(limitSwitchState);
}
