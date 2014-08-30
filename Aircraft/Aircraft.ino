#include <arduino.h>
#include <Wire.h>
#include <EEPROM.h>

#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>
#include <HMC5883L.h>

#include <SPI.h>
#include <NetDefine.h>
#include <arduino.h>

#include "power.h"
#include "control.h"
#include "connect.h"

Cpower * pPower;
Ccontrol * pControl;
Cconnect * conn;
void setup()
{
  Serial.begin(115200);
  conn =  new Cconnect(8, 7);
  pControl =  new Ccontrol(5, 6, 9, 10);
  pPower = new Cpower(2);
  Serial.println("Initializing finish");
}

NetStruct msg;
void loop()
{
  pControl->GetPitch();
  pControl->GetYaw();
  pControl->GetYaw();

  for (int i = 0; i < 10; i++)
  {
    conn->Read(&msg);
    float vf = 0;
    int vi = 0;
    Serial.println(msg.type);
    switch (msg.type)
    {
      case TYPE_UNKNOW:
        break;
      case TYPE_TIME_SYN:
        vf =  0;
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;
      case TYPE_POWER:
        vf = pPower->GetVoltage();
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;
      case TYPE_MOTOR_A:
        break;
      case TYPE_MOTOR_B:
        break;
      case TYPE_MOTOR_C:
        break;
      case TYPE_MOTOR_D:
        break;

      case TYPE_PITCH:
        vf =  pControl->GetPitch();
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;
      case TYPE_PITCH_P:
        vf =  0;
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;
      case TYPE_PITCH_I:
        vf =  0;
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;
      case TYPE_PITCH_D:
        vf =  0;
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;

      case TYPE_ROLL:
        vf = pControl->GetRoll();
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;
      case TYPE_ROLL_P:
        vf =  0;
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;
      case TYPE_ROLL_I:
        vf =  0;
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;
      case TYPE_ROLL_D:
        vf =  0;
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;

      case TYPE_YAW:
        vf = pControl->GetYaw();
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;
      case TYPE_YAW_P:
        vf =  0;
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;
      case TYPE_YAW_I:
        vf =  0;
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;
      case TYPE_YAW_D:
        vf =  0;
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;

      case TYPE_ELEVATION:
        break;
      case TYPE_ELEVATION_P:
        break;
      case TYPE_ELEVATION_I:
        break;
      case TYPE_ELEVATION_D:
        break;

      case TYPE_UNLOCK:
        vf =  0;
        msg.value = *(ulong*)& vf;
        Serial.println("unlock");
        pControl->UnlockMotor();
        break;
      case TYPE_SELF_STATIONARY:
        vf =  0;
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        break;
      case TYPE_LAND:
        break;
      case TYPE_INIT_BALANCE:
        vf =  0;
        msg.value = *(ulong*)& vf;
        conn->Send(&msg);
        pControl->BalanceAdjust();
        break;
      case TYPE_READ_CONFIG:
        break;
      case TYPE_WRITE_CONFIG:
        break;

      default:
        break;
    }
  }
}
