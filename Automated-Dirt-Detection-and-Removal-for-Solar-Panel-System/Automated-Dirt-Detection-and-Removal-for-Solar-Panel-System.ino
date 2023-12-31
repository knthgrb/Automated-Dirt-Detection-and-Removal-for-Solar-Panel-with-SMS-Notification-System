#include <Wire.h>
#include <Adafruit_TCS34725.h>

// Declarations
#define GSM_SERIAL Serial1
const int motorEnablePin = 8; // Speed control for motor
const int motorAInput1 = 3;
const int motorAInput2 = 2;
const int limitSwitchPin1 = 6;
const int limitSwitchPin2 = 7;
const int relayPin = 5;
const int waterSensorPin = A0;

// GSM module set-up
const char* phone_number = "+639xxxxxxxxx"; // Replace with your phone number

Adafruit_TCS34725 tcs[4] = {
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_16X),
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_16X),
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_16X),
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_16X)
};

// Variables to store clean panel color values
int cleanPanelColor[4][3] = {{0, 0, 0},
                             {0, 0, 0},
                             {0, 0, 0},
                             {0, 0, 0}};

bool cleaningInProgress = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing...");

  // Start I2C communication with the Multiplexer
  Wire.begin();
  initializeSensors();

  // motor, relay, limit switch, and water sensor setup
  pinMode(motorEnablePin, OUTPUT);
  pinMode(motorAInput1, OUTPUT);
  pinMode(motorAInput2, OUTPUT);
  pinMode(limitSwitchPin1, INPUT_PULLUP);
  pinMode(limitSwitchPin2, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  pinMode(waterSensorPin, INPUT);

  // Set the motor to the initial position
  digitalWrite(motorAInput1, LOW);
  digitalWrite(motorAInput2, LOW);
  analogWrite(motorEnablePin, 0);

  // Set the relay to off initially
  digitalWrite(relayPin, LOW);

  Serial.println("Initialization complete.");
}

void loop()
{
  if (!cleaningInProgress)
  {
    getRawDataFromSensors();

    // Check for color difference and initiate cleaning if necessary
    if (isCleaningNeeded())
    {
      cleaningInProgress = true;
      sendSMS("Cleaning in progress...");
      cleanSolarPanel();
      sendSMS("Cleaning completed.");
    }
  }

  // Check water level
  if (isWaterLevelLow())
  {
    sendSMS("Water level is low. Refill the water tank.");
  }

  delay(1000);
}

// Function to select I2C BUS
void TCA9548A(uint8_t bus)
{
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // Send byte to select bus
  Wire.endTransmission();
}

// Function to initialize color sensors
void initializeSensors()
{
  for (int sensorNum = 0; sensorNum < 4; sensorNum++)
  {
    TCA9548A(sensorNum + 2); // Bus numbers start from 2

    if (!tcs[sensorNum].begin(0x29))
    {
      Serial.print("Could not find a valid color sensor on bus ");
      Serial.println(sensorNum + 2);
      while (1);
    }
  }

  delay(500);

  // Store clean panel color values
  for (int sensorNum = 0; sensorNum < 4; sensorNum++)
  {
    TCA9548A(sensorNum + 2); // Bus numbers start from 2
    readColorValues(sensorNum, cleanPanelColor[sensorNum]);
  }
}

// Function to read color values from a sensor
void readColorValues(int sensorNum, int *color)
{
  TCA9548A(sensorNum + 2); // Bus numbers start from 2
  uint16_t r, g, b, c;
  tcs[sensorNum].getRawData(&r, &g, &b, &c);
  color[0] = r;
  color[1] = g;
  color[2] = b;
}

// Function to get raw data from sensors
void getRawDataFromSensors()
{
  if (!cleaningInProgress)
  {
    Serial.println("Raw Data from Color Sensors:");

    for (int sensorNum = 0; sensorNum < 4; sensorNum++)
    {
      TCA9548A(sensorNum + 2); // Bus numbers start from 2
      uint16_t r, g, b, c;
      tcs[sensorNum].getRawData(&r, &g, &b, &c);
      Serial.print("Sensor ");
      Serial.print(sensorNum + 1);
      Serial.print(" - R: ");
      Serial.print(r);
      Serial.print(" G: ");
      Serial.print(g);
      Serial.print(" B: ");
      Serial.println(b);
    }

    Serial.println();
  }
}

// Function to check if cleaning is needed
bool isCleaningNeeded()
{
  // Adjust the threshold based on your application
  int colorThreshold = 300;

  for (int sensorNum = 0; sensorNum < 4; sensorNum++)
  {
    int currentColor[3];
    readColorValues(sensorNum, currentColor);

    for (int i = 0; i < 3; i++)
    {
      if (abs(cleanPanelColor[sensorNum][i] - currentColor[i]) > colorThreshold)
      {
        Serial.print("Color difference: ");
        Serial.println(abs(cleanPanelColor[sensorNum][i] - currentColor[i]));
        Serial.println("Cleaning needed!");
        return true;
      }
    }
  }

  return false;
}

// Function to clean the solar panel
void cleanSolarPanel()
{
  Serial.println("Cleaning the solar panel...");

  // Activate the motor and water pump
  digitalWrite(relayPin, HIGH); // Turn on the water pump
  delay(1000); // Delay to ensure water pump is activated before moving the motor

  // Move the motor until it reaches the limit switch on the other side
  while (!digitalRead(limitSwitchPin1))
  {
    digitalWrite(motorAInput1, HIGH); // Set motor direction to move away from limitSwitchPin1
    digitalWrite(motorAInput2, LOW);
    analogWrite(motorEnablePin, 200); // Adjust speed as needed
    delay(10); // Small delay to allow the loop to iterate and check the limit switch
  }

  // Stop the motor
  analogWrite(motorEnablePin, 0);
  delay(3000);

  // Reverse the motor direction
  digitalWrite(motorAInput1, LOW);
  digitalWrite(motorAInput2, HIGH);

  // Continue until the motor reaches the limit switch at the starting position
  while (!digitalRead(limitSwitchPin2))
  {
    analogWrite(motorEnablePin, 200); // Adjust speed as needed
    delay(10); // Small delay to allow the loop to iterate and check the limit switch
  }

  // Stop the motor and turn off the water pump
  analogWrite(motorEnablePin, 0);
  digitalWrite(relayPin, LOW);

  delay(3000);

  // After cleaning, update the clean panel color values
  for (int sensorNum = 0; sensorNum < 4; sensorNum++)
  {
    readColorValues(sensorNum, cleanPanelColor[sensorNum]);
  }

  cleaningInProgress = false; // Reset the flag after cleaning is complete
}


// Function to send an SMS
void sendSMS(const char* message)
{
  GSM_SERIAL.begin(9600);
  delay(1000);

  // Enable text mode
  GSM_SERIAL.println("AT+CMGF=1");
  delay(500);

  // Set the phone number
  GSM_SERIAL.print("AT+CMGS=\"");
  GSM_SERIAL.print(phone_number);
  GSM_SERIAL.println("\"");
  delay(500);

  // Send the message
  GSM_SERIAL.println(message);
  delay(500);

  // Send Ctrl+Z to end the message
  GSM_SERIAL.println((char)26);
  delay(500);

  // Wait for the module to send the SMS
  delay(3000);

  // Disable text mode
  GSM_SERIAL.println("AT+CMGF=0");
  delay(500);

  GSM_SERIAL.end();
}

// Function to check water level
bool isWaterLevelLow()
{
  int waterLevel = analogRead(waterSensorPin);
  Serial.print("waterLevel: ");
  Serial.println(waterLevel);
  waterLevel < 300 ? Serial.println("needs refill") : Serial.println(NULL);
  // Adjust the threshold when needed
  return waterLevel < 300;
}

