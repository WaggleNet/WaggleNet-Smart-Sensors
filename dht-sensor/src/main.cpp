#include <Arduino.h>
#define UNO
#include "pnpsensor.h"

#include "DHT.h"

#define DHTPIN A1     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

PnPSensor sensor;
DHT dht(DHTPIN, DHTTYPE);

float temperature = 0;
float humidity = 0;

void getTemperature() {
  temperature = dht.readTemperature(true);
}

void getHumidity() {
  humidity = dht.readHumidity();
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("Sensor is setting up"));
  sensor.type = 1001L;
  sensor.addAutoEntry(temperature, getTemperature);
  sensor.addAutoEntry(humidity, getHumidity);
  Serial.println(F("Sensor has started"));
  uint8_t address = readAddress();
  Serial.print("-->\tSystem.Address\t0x");
  Serial.println(address, HEX);
  sensor.begin(address);
  dht.begin();
  StartSensor(sensor);
}

void loop() {
  sensor.update();
}