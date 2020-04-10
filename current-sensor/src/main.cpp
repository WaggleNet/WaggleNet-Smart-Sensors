#include <Arduino.h>
#define UNO
#define DEBUG
#include "pnpsensor.h"

#include "Adafruit_INA219.h"
#include "SoftwareWire.h"

PnPSensor sensor;
SoftwareWire myWire(A0,A1); // sda=A0 scl=A1
Adafruit_INA219 ina219(0x41); // 0x40,41,44,45

float current = 1;
float voltage = 1;
float constant = 42;

void getCurrent() {
  current = ina219.getCurrent_mA();
  // Serial.println(current);
}

void getVoltage() {
  float busvoltage = ina219.getBusVoltage_V();
  float shuntvoltage = ina219.getShuntVoltage_mV();
  voltage = busvoltage + shuntvoltage/1000;
  // Serial.print("voltage: ");
  // Serial.println(voltage);
  // Serial.print("busvoltage: ");
  // Serial.println(busvoltage);
  // Serial.print("shuntvoltage: ");
  // Serial.println(shuntvoltage);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Sensor is setting up");
  sensor.type = 0x1001L;
  sensor.addAutoFloatEntry(current, getCurrent);
  sensor.addAutoFloatEntry(voltage, getVoltage);
  // sensor.addFloatEntry(voltage);
  sensor.addFloatEntry(constant);
  Serial.println("Sensor has started");
  sensor.begin(readAddress());
  ina219.begin(&myWire);
  ina219.setCalibration_16V_400mA();
  StartSensor(sensor);
}

void loop() {
  // delay(2000);
  sensor.update();
  // Serial.println("update");
}
