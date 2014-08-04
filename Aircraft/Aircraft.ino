#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

#include "init.h" 
#include "ypr.h"

void setup()
{
  InitConfiguration();
}

void loop()
{
  float ypr[3];
  GetYPR(&ypr[0]);
  Serial.print(ypr[0]);
  Serial.print("\t");
  Serial.print(ypr[1]);
  Serial.print("\t");
  Serial.print(ypr[2]);
  Serial.print("\n");
}








