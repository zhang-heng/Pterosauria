#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

MPU6050 mpu;

void GetYPR(float * pYPR)
{
  float ypr[3]; 
  uint8_t mpuIntStatus = mpu.getIntStatus();
  uint8_t fifoCount = mpu.getFIFOCount();
  uint16_t packetSize =mpu.dmpGetFIFOPacketSize(); 
  Quaternion q;
  VectorFloat gravity; 
  uint8_t fifoBuffer[64];

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


