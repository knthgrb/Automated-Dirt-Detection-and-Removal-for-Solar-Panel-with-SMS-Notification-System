//if using arduino uno, use Software Serial Library then use 2 digital pins
//#include<SoftwareSerial.h>

// SoftwareSerial gsm(RX, TX); replace with pin numbers

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);  // Use Serial1 for communication with GSM module

  Serial.println("Initializing GSM module...");
  delay(1000);

  // Initialize the GSM module
  gsmInit();

  Serial.println("GSM module initialized.");
}

void loop() {
  // Send a test SMS
  sendTestSMS();

  // Wait for 10 seconds before making a call
  delay(10000);

  // Make a test call
  makeTestCall();
}

void gsmInit() {
  Serial1.println("AT"); // Send AT command to check communication
  delay(1000);

  // You may need to configure your SIM card with PIN, if required
  // Serial1.println("AT+CPIN=your_pin");

  // Set SMS mode to text
  Serial1.println("AT+CMGF=1");
  delay(1000);
}

void sendTestSMS() {
  Serial.println("Sending test SMS...");

  // Replace with the phone number you want to send the SMS to
  Serial1.println("AT+CMGS=\"639xxxxxxxxx\"");
  delay(1000);

  // Message
  Serial1.print("test");
  delay(1000);

  // Send Ctrl+Z to end the SMS
  Serial1.write(26);
  delay(1000);
}

void makeTestCall() {
  Serial.println("Making a test call...");

  // Replace with the phone number you want to call
  Serial1.println("639xxxxxxxxxx;");
  delay(10000); // Wait for 10 seconds (adjust as needed)

  // Hang up the call
  Serial1.println("ATH");
  delay(1000);
}
