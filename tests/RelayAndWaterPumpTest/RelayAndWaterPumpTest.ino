// Relay
const int relayPin = 5;   // Digital pin to connect the relay

void setup() {
  pinMode(relayPin, OUTPUT);

  // Initially, turn off the relay
  turnOffRelay();
}

void loop() {
  // Turn on the relay
  turnOnRelay();

  delay(5000);

  turnOffRelay();
}

void turnOnRelay() {
  digitalWrite(relayPin, HIGH);
}

void turnOffRelay() {
  digitalWrite(relayPin, LOW);
}
