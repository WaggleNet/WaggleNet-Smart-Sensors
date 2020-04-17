#include <Arduino.h>
#define UNO
// #define DEBUG
#include "pnpsensor.h"

#include "Adafruit_INA219.h"
#include "SoftwareWire.h"

PnPSensor sensor;
SoftwareWire myWire(A0,A1);     // sda=A0 scl=A1
Adafruit_INA219 ina219(0x41);   // 0x40,41,44,45

float current = 0;
float voltage = 0;

void getCurrent() {
  current = ina219.getCurrent_mA();
}

// Voltage between Gnd and Vin+ if test and supply voltage share Gnd. Not that accurate
void getVoltage() {
  voltage = ina219.getBusVoltage_V() + ina219.getShuntVoltage_mV()/1000;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Sensor is setting up");
  sensor.type = 0x1001L;
  sensor.addAutoFloatEntry(current, getCurrent);
  sensor.addAutoFloatEntry(voltage, getVoltage);
  Serial.println("Sensor has started");
  uint8_t address = readAddress();
  Serial.print("-->\tSystem.Address\t0x");
  Serial.println(address, HEX);
  sensor.begin(address);
  ina219.begin(&myWire);
  ina219.setCalibration_16V_400mA();      // setCalibration_32V_2A() setCalibration_32V_1A()
  StartSensor(sensor);
}

void loop() {
  sensor.update();
}
