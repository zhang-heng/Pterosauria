#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
MPU6050 mpu;
uint16_t packetSize;

uint8_t mpuIntStatus ;
uint8_t fifoCount;
Quaternion q;
VectorFloat gravity; 
uint8_t fifoBuffer[64];

volatile bool mpuInterrupt = false; 
void dmpDataReady() {
  mpuInterrupt = true;
}

void initYPR()
{
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  mpu.initialize();
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
  mpu.setDMPEnabled(true);
  mpuIntStatus = mpu.getIntStatus();
  packetSize = mpu.dmpGetFIFOPacketSize();
}

void GetYPR(float * pYPR)
{
  while(!mpuInterrupt && fifoCount < packetSize) {};
  fifoCount = mpu.getFIFOCount();
  float ypr[3]; 

  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));
  } 
  else if (mpuIntStatus & 0x02) {
    while (fifoCount < packetSize) 
      fifoCount = mpu.getFIFOCount();

    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    pYPR[0] = ypr[0] * 180/M_PI;
    pYPR[1] = ypr[1] * 180/M_PI;
    pYPR[2] = ypr[2] * 180/M_PI;
  }
}








