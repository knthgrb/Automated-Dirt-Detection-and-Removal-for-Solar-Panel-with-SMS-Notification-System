#include<Wire.h>
#include <Adafruit_TCS34725.h>

//initializing four instances of the Adafruit_TCS34725 class for four color sensors
Adafruit_TCS34725 tcs1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 tcs2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 tcs3 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 tcs4 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");
  
  // start I2C communication with the Multiplexer
  Wire.begin();
  //  init sensor on bus number 2
  TCA9548A(2);
  if (!tcs1.begin(0x29)) {
    Serial.println("Could not find a valid color sensor on bus 2");
    while (1);
  }
  Serial.println();
  
  // // init sensor on bus number 3
  TCA9548A(3);
  if (!tcs2.begin(0x29)) {
    Serial.println("Could not find a valid color sensor on bus 3");
    while (1);
  }
  Serial.println();
  
  // // init sensor on bus number 4
  TCA9548A(4);
  if (!tcs3.begin(0x29)) {
    Serial.println("Could not find a valid color sensor on bus 4");
    while (1);
  }
  Serial.println();
  
  // // init sensor on bus number 5
  TCA9548A(5);
  if (!tcs4.begin(0x29)) {
    Serial.println("Could not find a valid color sensor on bus 5");
    while (1);
  }

  Serial.println("Initialization complete.");
}

void loop() {
  getRawDataFromSensors();
  delay(3000);
}

// function to select I2C BUS
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  Serial.println(bus);
}


void getRawDataFromSensors() {
  Serial.println("Raw Data from Color Sensors:");
  uint16_t r, g, b, c;

  TCA9548A(2);
  tcs1.getRawData(&r, &g, &b, &c);
  Serial.print("Sensor 1 - R: "); Serial.print(r); Serial.print(" G: "); Serial.print(g); Serial.print(" B: "); Serial.print(b); Serial.print(" C: "); Serial.println(c);

  TCA9548A(3);
  tcs2.getRawData(&r, &g, &b, &c);
  Serial.print("Sensor 2 - R: "); Serial.print(r); Serial.print(" G: "); Serial.print(g); Serial.print(" B: "); Serial.print(b); Serial.print(" C: "); Serial.println(c);

  TCA9548A(4);
  tcs3.getRawData(&r, &g, &b, &c);
  Serial.print("Sensor 3 - R: "); Serial.print(r); Serial.print(" G: "); Serial.print(g); Serial.print(" B: "); Serial.print(b); Serial.print(" C: "); Serial.println(c);

  TCA9548A(5);
  tcs4.getRawData(&r, &g, &b, &c);
  Serial.print("Sensor 4 - R: "); Serial.print(r); Serial.print(" G: "); Serial.print(g); Serial.print(" B: "); Serial.print(b); Serial.print(" C: "); Serial.println(c);

  Serial.println();
}