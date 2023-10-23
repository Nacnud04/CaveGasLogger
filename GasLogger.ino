#include <SoftwareSerial.h>

#include "DFRobot_SD3031.h" // package for RTC
#include "DFRobot_OxygenSensor.h" // package for O2 Sensor

// init UART for CO2 sensor
SoftwareSerial mySerial(10, 11); // RX TX
// hex data to retrieve a measurement from CO2 Sensor
unsigned char hexData[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

// init sensor/module classes
DFRobot_SD3031 rtc;
DFRobot_OxygenSensor oxygen;

// define parameters for O2 sensor
#define oxygen_addr ADDRESS_3 // loaded in from O2 package = 0x73
#define collect_num 10

void setup() {
  // init serial
  Serial.begin(9600);
  // begin serial connection with CO2 Sensor
  while (!Serial) {
  }
  mySerial.begin(9600);
  Serial.println("CO2 sensor successfully initalized");
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
