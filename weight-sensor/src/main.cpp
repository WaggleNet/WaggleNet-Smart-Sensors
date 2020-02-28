#include <Arduino.h>
#include <Wire.h>
//#define UNO
#include "pnpsensor.h"
#include "HX711.h"

#define SENSOR_TYPE 1004
#define LED_PIN 9
#define HX711CLK A2
#define HX711DATA A1

PnPSensor sensor;
HX711 scale;

uint32_t weight;

void get_weight_reading(){
  //digitalWrite(LED_PIN, HIGH);
  //Serial.println("starting");
  weight = scale.read_average(uint8_t(100));
  // Serial.println(weight);
  // Serial.println("done");
  //digitalWrite(LED_PIN, LOW);
  //weight = 0;
}

// uint32_t bla;
// void get_blablabla(){
//   bla = millis();
// }

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.begin(115200);
  Serial.println("-!>\tSTART\tSystem");
  scale.begin(HX711DATA, HX711CLK);
  sensor.type = SENSOR_TYPE;
  sensor.addAutoUintEntry(weight, get_weight_reading);
  // sensor.addAutoUintEntry(bla, get_blablabla);
  // sensor.addAutoUintEntry(weight, get_weight_reading);
  uint8_t address = readAddress();
  Serial.print("-->\tSystem.Address\t0x");
  Serial.println(address, HEX);
  sensor.begin(address);
  StartSensor(sensor);
  digitalWrite(LED_PIN, LOW);
  Serial.println("-!>\tOK\tSystem");
}

void loop() {
  sensor.update();
  // sensor.changed(1);
}
