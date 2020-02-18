#include <Arduino.h>
#include <Wire.h>
//#define UNO
#include "pnpsensor.h"

#define SENSOR_TYPE 1002
#define LED_INDICATOR 9
#define AMLIGHT A2
#define VIBSW A3

PnPSensor sensor;

uint32_t ambient_light;
uint8_t hasVibration = 0;

//new stuff
uint8_t vib_check = 0;
uint8_t reading_size = 100;

void get_ambient_light() {
  ambient_light = 1023-analogRead(AMLIGHT);
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

//new stuff
int get_vibration(){
  vib_check = 0;
  for(int i = 0; i < reading_size; i++){
      vib_check += digitalRead(VIBSW); 
      delay(1);
  }
  if(vib_check > 1){
      return 1;
  }
  else{
    return 0;
  } 
}

void loop() {
  sensor.update();
  hasVibration = get_vibration();
  sensor.changed(1);
}