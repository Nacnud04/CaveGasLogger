#include "DFRobot_OxygenSensor.h"

/**
 * i2c slave Address, The default is ADDRESS_3.
 * ADDRESS_0   0x70  i2c device address.
 * ADDRESS_1   0x71
 * ADDRESS_2   0x72
 * ADDRESS_3   0x73
 */
#define Oxygen_IICAddress ADDRESS_3
#define OXYGEN_CONECTRATION 20.9  // The current concentration of oxygen in the air.
#define OXYGEN_MV           0     // The value marked on the sensor, Do not use must be assigned to 0.
DFRobot_OxygenSensor oxygen;

void setup(void) 
{
  Serial.begin(9600);
  while(!oxygen.begin(Oxygen_IICAddress)){
    Serial.println("I2c device number error !");
    delay(1000);
  }
  Serial.println("I2c connect success !");
  
  /**
   * Choose method 1 or method 2 to calibrate the oxygen sensor.
   * 1. Directly calibrate the oxygen sensor by adding two parameters to the sensor.
   * 2. Leave O2 sensor steady for 10 minutes, 
   *    OXYGEN_CONECTRATION is the current concentration of oxygen in the air (20.9%mol except in special cases),
   *    Not using the first calibration method, the OXYGEN MV must be 0.
   */
  oxygen.calibrate(OXYGEN_CONECTRATION, OXYGEN_MV);
}

void loop(void)
{
  Serial.println("The oxygen sensor was calibrated successfully.");
  delay(1000);
}
