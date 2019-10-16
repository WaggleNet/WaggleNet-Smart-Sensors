#include <Arduino.h>
#include <Wire.h>
#define UNO
#include "pnpsensor.h"

#define SENSOR_TYPE 0x1002
#define LED_INDICATOR 9
#define AMLIGHT A3
#define VIBSW A2

PnPSensor sensor;

uint32_t ambient_light;
uint8_t hasVibration = 0;

void get_ambient_light() {
  ambient_light = analogRead(AMLIGHT);
}

uint8_t readAddress() {
  DDRD &= 0b10000111; // Clear PD6-3 to read address
  PORTD |= 0b01111000; // Set them to pullup
  delay(10);
  return ((PIND >> 3) & 0xf) + 0x20; // Take PD6-3 readouts
}

void setup() {
  pinMode(LED_INDICATOR, OUTPUT);
  digitalWrite(LED_INDICATOR, HIGH);
  pinMode(VIBSW, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("-!>\tSTART\tSystem");
  sensor.type = SENSOR_TYPE;
  sensor.addAutoEntry(ambient_light, get_ambient_light);
  sensor.addEntry(hasVibration);
  uint8_t address = readAddress();
  Serial.print("-->\tSystem.Address\t0x");
  Serial.println(address, HEX);
  sensor.begin(address);
  StartSensor(sensor);
  digitalWrite(LED_INDICATOR, LOW);
  Serial.println("-!>\tOK\tSystem");
}

void loop() {
  sensor.update();
  if (!digitalRead(VIBSW)) {
    hasVibration = 1;
    sensor.changed(1);
  }
}
