#include <Sensirion.h>
#include <DHT.h>

// Sensor pins and types
#define soilSensorDataPin   6
#define soilSensorClockPin  7
#define airSensorPin        4
#define airSensorType       DHT11

// Variables for the temperature & humidity sensor
float soilHumidity;
float soilTemp;
float soilDewpoint;
float airHumidity;
float airTemp;

// Create sensor instance
Sensirion soilSensor = Sensirion(soilSensorDataPin, soilSensorClockPin);
DHT airSensor = DHT(airSensorPin, airSensorType);

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");

  airSensor.begin();
}

void loop() {
  if (Serial.available()) {
    byte c = Serial.read();

    if (c == 'm') {
      readSensor();
      sendAndroidValues();
    }
  }
  
  // Wait 100 ms before next measurement
  delay(100);  
}

void readSensor() {
  soilSensor.measure(&soilTemp, &soilHumidity, &soilDewpoint);
  airHumidity = airSensor.readHumidity();
  airTemp = airSensor.readTemperature();
}

void printData() {
  //Serial.print("Soil Humidity: ");
  //Serial.print(soilHumidity);
  //Serial.print("%\t");
  //Serial.print("Soil Temperature: ");
  //Serial.print(soilTemp);
  //Serial.println("*C");
  Serial.print("Air Humidity: ");
  Serial.print(airHumidity);
  Serial.print("%\t");
  Serial.print("Air Temperature: ");
  Serial.print(airTemp);
  Serial.println("*C");
}

void sendAndroidValues() {
  Serial.print('#');
  Serial.print(airHumidity);
  Serial.print(airTemp);
  Serial.print(soilHumidity);
  Serial.print(soilTemp);
  Serial.print('~');
  Serial.println();
  delay(10);
}
