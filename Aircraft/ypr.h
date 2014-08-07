#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
MPU6050 mpu;  

class Cypr
{
private: 
  bool dmpReady = false;  
  uint8_t mpuIntStatus;  
  uint8_t devStatus; 
  uint16_t packetSize; 
  uint16_t fifoCount;
  uint8_t fifoBuffer[64];
  Quaternion q;
  VectorFloat gravity;
  float ypr[3];
  volatile bool mpuInterrupt = false;
  void initYPR()
  {
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    TWBR = 24;
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif
    Serial.println(F("MPU6050: Initializing I2C devices..."));
    mpu.initialize(); 
    Serial.println(F("MPU6050: Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050: connection successful") : F("MPU6050: connection failed")); 
    Serial.println(F("MPU6050: Initializing DMP..."));
    devStatus = mpu.dmpInitialize(); 
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);  
    if (devStatus == 0) { 
      Serial.println(F("MPU6050: Enabling DMP..."));
      mpu.setDMPEnabled(true); 
      Serial.println(F("MPU6050: Enabling interrupt detection (Arduino external interrupt 0)..."));
      attachInterrupt(0, dmpDataReady, RISING);
      mpuIntStatus = mpu.getIntStatus(); 
      Serial.println(F("MPU6050: DMP ready! Waiting for first interrupt..."));
      dmpReady = true; 
      packetSize = mpu.dmpGetFIFOPacketSize();
    } 
    else { 
      Serial.print(F("MPU6050: DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
    } 
  }

  void FlushYPR()
  {
    if (!dmpReady) return; 
    if (!mpuInterrupt && fifoCount < packetSize) return;

    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus(); 
    fifoCount = mpu.getFIFOCount(); 
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) 
    {
      mpu.resetFIFO();
      Serial.println(F("MPU6050: FIFO overflow!")); 
    } 
    else if (mpuIntStatus & 0x02) {
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount(); 
      mpu.getFIFOBytes(fifoBuffer, packetSize); 
      fifoCount -= packetSize;
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    }
  }
public:
  static Cypr* pCypr;
  static void dmpDataReady()
  {
    pCypr->mpuInterrupt = true;
  }

  Cypr()
  {
  }

  void Init()
  {
    initYPR();
    pCypr = this;    
  }

  float GetPitchPoint()
  {  
    FlushYPR();
    return ypr[1] * 180/M_PI;
  }

  float GetRollPoint()
  {
    FlushYPR();
    return ypr[2] * 180/M_PI;
  }
};







