#include <Arduino.h>
#define UNO
#include "pnpsensor.h"
#include "Ticker.h"

#define CHRG_CHARGING 7
#define CHRG_STANDBY 8

#define SOLAR_SENS A6
#define BATT_SENS A7

#define LED_BLUE 6
#define LED_ORANGE 4

float computeVoltage(int pin);
void updateSystemMonitor();

float solar_volt;
float battery_volt;
bool charging;
bool charge_ready;

PnPSensor sensor;
Ticker systemStatsTimer(updateSystemMonitor, 2000);

float computeVoltage(int pin){
  int sensorValue = analogRead(pin);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (1.1*5.3 / 1023.0);
  return voltage;
}

void updateSystemMonitor() {
  digitalWrite(LED_BLUE, HIGH);
  analogReference(INTERNAL);
  if (!(ADMUX >> REFS1)) {
      analogRead(A6);
      analogRead(A6);
      analogRead(A6);
      analogRead(A6);
      analogRead(A6);
  }   // Get my voltage right

  battery_volt = computeVoltage(BATT_SENS);
  solar_volt = computeVoltage(SOLAR_SENS);
  charging = !digitalRead(CHRG_CHARGING);
  charge_ready = !digitalRead(CHRG_STANDBY);
  sensor.changed(0);
  sensor.changed(1);
  sensor.changed(2);
  sensor.changed(3);
  digitalWrite(LED_BLUE, LOW);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Sensor is setting up");

  // Entries follow the node
  sensor.type = 0x1a;  // WaggleNode v1 Telemetry Format
  sensor.addFloatEntry(battery_volt);
  sensor.addFloatEntry(solar_volt);
  sensor.addBoolEntry(charging);
  sensor.addBoolEntry(charge_ready);

  uint8_t address = readAddress();
  
  Serial.println("Sensor has started");
  sensor.begin(address);
  StartSensor(sensor);
}

void loop() {
  sensor.update();
  systemStatsTimer.update();
}
