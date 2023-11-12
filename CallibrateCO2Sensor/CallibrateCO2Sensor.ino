#include <SoftwareSerial.h> // for CO2 sensor

// init UART for CO2 sensor
SoftwareSerial mySerial(8, 9); // RX TX
// hex data to retrieve a measurement from CO2 Sensor
unsigned char hexData[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
// hex to disable self callibration
unsigned char disSelfCal[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};
// hex for zero point 
unsigned char setZero[9] = {0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println(F("CO2 sensor successfully initalized"));
  Serial.println(F("Disabling self callibration"));
  mySerial.write(disSelfCal, 9);
  Serial.println(F("Waiting 20 minutes before setting zero point to 400ppm"));
  Serial.println(F("During this period ensure the device is in a stable 400ppm environment"));
  float percent;
  for (int i = 0; i < 60*20; i++) {
    // Move the cursor to the beginning of the line
    Serial.print("\r");
    Serial.print(i);
    Serial.print("s waited out of 1200, or ");
    percent = 100.0*float(i)/1200.0;
    Serial.print(percent);
    Serial.println("%                        ");
    Serial.flush();
    delay(1000);
  }
  Serial.println(F("Performing zero callibration"));
  mySerial.write(setZero, 9);
  Serial.println(F("Successfully performed zero callibration"));
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
