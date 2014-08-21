#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Wire.h>
#include <EEPROM.h>

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#include "control.h"
#include "connect.h"

Ccontrol * pControl;
Cconnect * conn;

void setup()
{
  Serial.begin(115200); 
  pControl =  new Ccontrol(5, 6, 9, 10); 
  conn =  new Cconnect(8, 7);
}

void loop()
{
  conn->Handle();
}

