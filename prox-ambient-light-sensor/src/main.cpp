#include <Arduino.h>
#define UNO
// #define DEBUG
#include "pnpsensor.h"

#include "APDS9930.h"
#include "SoftwareWire.h"

PnPSensor sensor;
APDS9930 apds(A0,A1);     // sda=A0 scl=A1

float ambient_light = 0;

void getAmbientLight() {
  apds.readAmbientLightLux(ambient_light);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Sensor is setting up");
  sensor.type = 0x1001L;
  sensor.addAutoFloatEntry(ambient_light, getAmbientLight);
  Serial.println("Sensor has started");
  uint8_t address = readAddress();
  Serial.print("-->\tSystem.Address\t0x");
  Serial.println(address, HEX);
  sensor.begin(address);
  apds.init();
  apds.enableLightSensor();
  StartSensor(sensor);
}

void loop() {
  sensor.update();
}
