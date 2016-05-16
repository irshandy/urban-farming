#include <Sensirion.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>

// Sensor pins and types
#define ledPin              5
#define soilSensorDataPin   6
#define soilSensorClockPin  7
#define airSensorPin        4
#define airSensorType       DHT11

// Plant types
#define babyLeafLettuce 0
#define cabbage         1
#define basil           2

// Variables for the temperature & humidity sensor
float soilHumidity;
float soilTemp;
float soilDewpoint;
float airHumidity;
float airTemp;

// Create sensor and LED instance
Sensirion soilSensor = Sensirion(soilSensorDataPin, soilSensorClockPin);
DHT airSensor = DHT(airSensorPin, airSensorType);
Adafruit_NeoPixel led = Adafruit_NeoPixel(60, ledPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");

  airSensor.begin();

  led.begin();
  led.show();
}

void loop() {
  if (Serial.available()) {
    byte c = Serial.read();

    if (c == 'm') {
      readSensor();
      sendAndroidValues();
    }
  }

  colorWipe(basil);
  
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

void colorWipe(int plantType) {
  switch (plantType) {
    case 0:
      for(int i=0; i<led.numPixels(); i++) {
        led.setPixelColor(i, led.Color(72, 255, 0));
        led.show();
        delay(50);
      }
      break;
    case 1:
      for(int i=0; i<led.numPixels(); i++) {
        led.setPixelColor(i, led.Color(255, 0, 0));
        led.show();
        delay(50);
      }
      break;
    case 2:
      for(int i = 0; i<led.numPixels(); i++) {
        if ((i + 1) % 4 == 0)
          led.setPixelColor(i, led.Color(0, 102, 255));
        else
          led.setPixelColor(i, led.Color(255, 0, 0));
        led.show();
        delay(50);
      }
      break;
  }
}
