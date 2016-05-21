#include <Sensirion.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>

// Sensor pins and types
#define ledPin              5
#define soilSensorDataPin   6
#define soilSensorClockPin  7
#define airSensorPin        4
#define airSensorType       DHT11
#define numLED              30

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

// Variables for LED
long timePassed = 0;
long timeStart = 0;
int onOff = 0;
boolean pastLEDStatus = true; 
int wavelength = 0;

// Variables for Android
String intData = "";
char intBuffer[12];

// Create sensor and LED instance
Sensirion soilSensor = Sensirion(soilSensorDataPin, soilSensorClockPin);
DHT airSensor = DHT(airSensorPin, airSensorType);
Adafruit_NeoPixel led = Adafruit_NeoPixel(numLED, ledPin, NEO_GRB + NEO_KHZ800);

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
    } else if ( c >= '0' && c <= '9') {
      intData += (char) c;
    } else if ( c == 'i') {
      intData.toCharArray(intBuffer, intData.length() + 1);
      int wavelength = atoi(intBuffer);
      Serial.println(wavelength);
      if(wavelength == 660){
        setLEDColor(babyLeafLettuce);
      } else if(wavelength == 530){
        setLEDColor(cabbage);
      } else if(wavelength == 661){
        setLEDColor(basil);
      }
      onOff = 1;
      
      intData = "";
    }
  }
  
  switchLED();
  
  // Wait 100 ms before next measurement
  delay(100);  
}

void switchLED() {
  timePassed += millis() - timeStart;
  if (timePassed >= 60 * 60 * 8) {
    timePassed = 0;
    if (pastLEDStatus == true) {
      led.setBrightness(0);
      led.show();
      pastLEDStatus = false;
      onOff = 0;
    }
    else {
      led.setBrightness(255);
      led.show();
      pastLEDStatus = true;
      onOff = 1;
    }
  }
}

void readSensor() {
  soilSensor.measure(&soilTemp, &soilHumidity, &soilDewpoint);
  airHumidity = airSensor.readHumidity();
  airTemp = airSensor.readTemperature();
}

void printData() {
  Serial.print("Soil Humidity: ");
  Serial.print(soilHumidity);
  Serial.print("%\t");
  Serial.print("Soil Temperature: ");
  Serial.print(soilTemp);
  Serial.println("*C");
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
  Serial.print('#');
  Serial.print(airTemp);
  Serial.print('#');
  Serial.print(soilHumidity);
  Serial.print('#');
  Serial.print(soilTemp);
  Serial.print('#');
  Serial.print(onOff);
  Serial.print('~');
  Serial.println();
  delay(10);
}

void setLEDColor(int plantType) {
  led.setBrightness(255);
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