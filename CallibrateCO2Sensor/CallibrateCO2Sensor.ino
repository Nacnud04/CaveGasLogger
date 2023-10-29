#include <SoftwareSerial.h> // for CO2 sensor

// init UART for CO2 sensor
SoftwareSerial mySerial(8, 9); // RX TX
// hex data to retrieve a measurement from CO2 Sensor
unsigned char hexData[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println(F("CO2 sensor successfully initalized"));
}

long hi, lo, CO2;
int measureCO2() {
  mySerial.write(hexData, 9);
  for (int i = 0, j = 0; i < 9; i++) {
    if (mySerial.available() > 0) {
      int ch = mySerial.read();
      if (i == 2) {
        hi = ch;    // high concentration
      }
      if (i == 3) {
        lo = ch;    // low concentration
      }
      if (i == 8) {
        CO2 = hi * 256 + lo; // true CO2 concentration
      }
    }
  }
  return CO2;
}

void loop(){
  Serial.print("CO2:");
  Serial.println(measureCO2());
  delay(1000);
}
