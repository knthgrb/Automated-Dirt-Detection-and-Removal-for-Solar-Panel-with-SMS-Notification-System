// Motor A
const int motorAEnablePin = 8;  // ENA - Speed control
const int motorAInput1 = 3;     // IN1
const int motorAInput2 = 2;     // IN2

void setup() {
  pinMode(motorAEnablePin, OUTPUT);
  pinMode(motorAInput1, OUTPUT);
  pinMode(motorAInput2, OUTPUT);

  // Initially, stop the motor
  stopMotor();
}

void loop() {
  // Run the motor forward
  moveForward();
  delay(5000);
  // Stop the motor
  stopMotor();
}

void moveForward() {
  digitalWrite(motorAInput1, HIGH);
  digitalWrite(motorAInput2, LOW);
  analogWrite(motorAEnablePin, 255); // Adjust speed by changing PWM value (0 to 255)
}

void stopMotor() {
  digitalWrite(motorAInput1, LOW);
  digitalWrite(motorAInput2, LOW);
  analogWrite(motorAEnablePin, 0);  // Set speed to 0 to stop the motor
}
