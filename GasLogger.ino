#include "DFRobot_SD3031.h" // package for RTC
#include "DFRobot_OxygenSensor.h" // package for O2 Sensor

// init sensor/module classes
DFRobot_SD3031 rtc;
DFRobot_OxygenSensor oxygen;

// define parameters for O2 sensor
#define oxygen_addr ADDRESS_3 // loaded in from O2 package = 0x73
#define collect_num 10

void setup() {
  // init serial
  Serial.begin(9600);
  // test & init RTC
  while (rtc.begin() != 0) {
    Serial.println("Failed to initalize RTC");
  }
  Serial.println("RTC successfully initalized");
  // test & init O2 sensor
  while (!oxygen.begin(oxygen_addr)) {
    Serial.println("Failed to initalize O2 sensor");
  }
  Serial.println("O2 sensor successfully initalized");
}

void loop() {

}
