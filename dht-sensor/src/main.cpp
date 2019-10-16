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
  uint8_t addr = 0x60;
  // Get address from PB3 to PB6
  addr += (PIND >> 3) & 0b1111;
  Serial.print(F("Address: 0x"));
  Serial.println(addr);
  sensor.begin(addr);
  dht.begin();
  StartSensor(sensor);
}

void loop() {
  sensor.update();
}