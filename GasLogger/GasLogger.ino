#include <SoftwareSerial.h> // for CO2 sensor
#include <SD.h> // SD card library

#include "DFRobot_SD3031.h" // package for RTC
#include "DFRobot_OxygenSensor.h" // package for O2 Sensor

// init UART for CO2 sensor
SoftwareSerial mySerial(8, 9); // RX TX
// hex data to retrieve a measurement from CO2 Sensor
unsigned char hexData[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

// init sensor/module classes
DFRobot_SD3031 rtc;
DFRobot_OxygenSensor oxygen;

// init file variable
File dataFile;

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
  Serial.println(F("CO2 sensor successfully initalized"));
  // test & init RTC
  while (rtc.begin() != 0) {
    Serial.println(F("Failed to initalize RTC"));
  }
  rtc.setHourSystem(rtc.e24hours);
  Serial.println(F("RTC successfully initalized"));
  // test & init O2 sensor
  while (!oxygen.begin(oxygen_addr)) {
    Serial.println(F("Failed to initalize O2 sensor"));
  }
  Serial.println(F("O2 sensor successfully initalized"));
  // test & init SD card
  if (!SD.begin(4)) {
    Serial.println(F("Failed to initalize SD card"));
    while (1);
  }
  Serial.println(F("SD Card sucessfully initalized"));
}

sTimeData_t sTime;
byte dateTime[6] = {0, 0, 0, 0, 0, 0};
void getTime() {
  sTime = rtc.getRTCTime();
  dateTime[0] = sTime.year - 2000;
  dateTime[1] = sTime.month;
  dateTime[2] = sTime.day;
  dateTime[3] = sTime.hour;
  dateTime[4] = sTime.minute;
  dateTime[5] = sTime.second;
}

int pSec = -1;
bool timeSet = false;
byte year;
byte month;
byte date;
byte dOW;
byte hour;
byte minute;
byte second;
void setTime(byte& year, byte& month, byte& date, byte& dOW,
                  byte& hour, byte& minute, byte& second) {
  // Call this if you notice something coming in on
  // the serial port. The stuff coming in should be in
  // the order YYMMDDwHHMMSSx
  boolean gotString = false;
  timeSet = true;
  char inChar;
  byte temp1, temp2;
  char inString[20];
  
  byte j=0;
  while (!gotString) {
    if (Serial.available()) {
      inChar = Serial.read();
      inString[j] = inChar;
      j += 1;
      if (inChar == 'x') {
        gotString = true;
      }
    }
    //if (getSec > pSec+10) { break; }
  }
  Serial.println(inString);
  // Read year first
  temp1 = (byte)inString[0] -48;
  temp2 = (byte)inString[1] -48;
  year = temp1*10 + temp2;
  // now month
  temp1 = (byte)inString[2] -48;
  temp2 = (byte)inString[3] -48;
  month = temp1*10 + temp2;
  // now date
  temp1 = (byte)inString[4] -48;
  temp2 = (byte)inString[5] -48;
  date = temp1*10 + temp2;
  // now Day of Week
  dOW = (byte)inString[6] - 48;
  // now hour
  temp1 = (byte)inString[7] -48;
  temp2 = (byte)inString[8] -48;
  hour = temp1*10 + temp2;
  // now minute
  temp1 = (byte)inString[9] -48;
  temp2 = (byte)inString[10] -48;
  minute = temp1*10 + temp2;
  // now second
  temp1 = (byte)inString[11] -48;
  temp2 = (byte)inString[12] -48;
  second = temp1*10 + temp2;
  rtc.setTime(2000+year,month,date,hour,minute,second);//Initialize time
  Serial.print("Sucessfully set time to: ");
  getTime();
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

float O2;
float measureO2() {
  O2 = oxygen.getOxygenData(collect_num);
  return O2;
}

String current_file = "";
String filepath = "";

void loop() {
  getTime();
  // open and write to a new file every day
  filepath = dateTime[0] + String("-") + dateTime[1] + String("-") + dateTime[2] + String(".csv");
  if ((filepath != current_file) && (22 < dateTime[0] < 26) && (0 < dateTime[1] < 13) && (0 < dateTime[2] < 32)) {
    current_file = filepath;
    if (dataFile) {
      dataFile.close();
    }
    dataFile = SD.open(current_file, FILE_WRITE);
    Serial.println(F("Writing new file"));
    dataFile.println(F("DTM, CO2(ppm), O2(%)"));
  }
  
  // every 30 seconds write out data by closing and opening file
  if (dateTime[5] % 30 == 0) {
    dataFile.close();
    dataFile = SD.open(current_file, FILE_WRITE);
  }
  
  // capture and write out data
  if (pSec != dateTime[5]) {
    Serial.print("CO2:");
    Serial.print(measureCO2());
    Serial.print("ppm | O2: ");
    Serial.print(measureO2());
    Serial.println(String("% @ ")+dateTime[3]+String(":")+dateTime[4]+String(":")+dateTime[5]);
    dataFile.println(dateTime[3]+String(":")+dateTime[4]+String(":")+dateTime[5]+String(",")+measureCO2()+String(",")+measureO2());
    pSec = dateTime[5];
  }
  
  //check to see if time needs to be updated
  if (Serial.available() && (timeSet == false)) {
    setTime(year, month, date, dOW, hour, minute, second);
  }
}
