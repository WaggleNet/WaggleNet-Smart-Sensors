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
  Serial.println("starting");
  //digitalWrite(LED_PIN, HIGH); //testing purposes: turns on led to show how long it takes to read
  weight = scale.read_average(uint8_t(20));
  weight = 0xffffffff - weight; //flip
  weight = int((weight/5366.1)-5.9926); //convert to pounds, remove if you want raw value
  //digitalWrite(LED_PIN, LOW); //testing purposes: turns of led when done reading
}


void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.begin(115200);
  Serial.println("-!>\tSTART\tSystem");
  scale.begin(HX711DATA, HX711CLK, 64);
  sensor.type = SENSOR_TYPE;
  sensor.addAutoUintEntry(weight, get_weight_reading);
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