#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>

#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>
#include <HMC5883L.h>
#include <NewPing.h>

#include <SPI.h>
#include <NetDefine.h>

#include "power.h"
#include "control.h"
#include "connect.h"

//获取电源电压
Cpower * pPower;
//控制电机
Ccontrol * pControl;
//网络
Cconnect * conn;

//初始化飞控
void setup()
{
  Serial.begin(115200);
  conn =  new Cconnect(8, 7);
  pControl =  new Ccontrol(5, 6, 9, 10);
  pPower = new Cpower(2);
  Serial.println("Initializing finish");
}

//网络接收数据包
NetStruct msg;

//循环体
void loop()
{
  pControl->GetPitch();
  pControl->GetYaw();

  for (int i = 0; i < 10; i++){
    conn->Read(&msg);
    float vf = 0;
    int vi = 0;
    switch (msg.type){
    case TYPE_TIME_SYN://获取飞控运行时间
      vf =  millis();
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_POWER://获取电池百分比
      vf = pPower->GetVoltage();
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_MOTOR_A://调整/获取A电调频率
      pControl->Speeds[0] += *(int*)&msg.value;
      vf = pControl->Speeds[0];
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_MOTOR_B://调整/获取B电调频率
      pControl->Speeds[1] +=  *(int*)&msg.value;
      vf = pControl->Speeds[1];
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_MOTOR_C://调整/获取C电调频率
      pControl->Speeds[2] +=  *(int*)&msg.value;
      vf = pControl->Speeds[2];
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_MOTOR_D://调整/获取D电调频率
      pControl->Speeds[3] +=  *(int*)&msg.value;
      vf = pControl->Speeds[3];
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;

    case TYPE_PITCH://调整/获取俯仰值 pid
      vf =  pControl->GetPitch();
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_PITCH_P:
      pControl->configPID.PitchP +=vf;
      msg.value = *(ulong*)& pControl->configPID.PitchP;
      conn->Send(&msg);
      break;
    case TYPE_PITCH_I:
      pControl->configPID.PitchI +=vf;
      msg.value = *(ulong*)& pControl->configPID.PitchI;
      conn->Send(&msg);
      break;
    case TYPE_PITCH_D:
      pControl->configPID.PitchD +=vf;
      msg.value = *(ulong*)& pControl->configPID.PitchD;
      conn->Send(&msg);
      break;

    case TYPE_ROLL://调整/获取横滚值 pid
      vf = pControl->GetRoll();
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_ROLL_P:
      pControl->configPID.RollP +=vf;
      msg.value = *(ulong*)& pControl->configPID.RollP;
      conn->Send(&msg);
      break;
    case TYPE_ROLL_I:
      pControl->configPID.RollI +=vf;
      msg.value = *(ulong*)& pControl->configPID.RollI;
      conn->Send(&msg);
      break;
    case TYPE_ROLL_D:
      pControl->configPID.RollD +=vf;
      msg.value = *(ulong*)& pControl->configPID.RollD;
      conn->Send(&msg);
      break;

    case TYPE_YAW://调整/获取航向值 pid
      vf = pControl->GetYaw();
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_YAW_P:
      pControl->configPID.YawP +=vf;
      msg.value = *(ulong*)& pControl->configPID.YawP;
      conn->Send(&msg);
      break;
    case TYPE_YAW_I:
      pControl->configPID.YawI +=vf;
      msg.value = *(ulong*)& pControl->configPID.YawI;
      conn->Send(&msg);
      break;
    case TYPE_YAW_D:
      pControl->configPID.YawD +=vf;
      msg.value = *(ulong*)& pControl->configPID.YawD;
      conn->Send(&msg);
      break;

    case TYPE_ELEVATION://调整/获取高度值  pid
      conn->Send(&msg);
      break;
    case TYPE_ELEVATION_P:
      pControl->configPID.ElevationP +=vf;
      msg.value = *(ulong*)& pControl->configPID.ElevationP;
      conn->Send(&msg);
      break;
    case TYPE_ELEVATION_I:
      pControl->configPID.ElevationI +=vf;
      msg.value = *(ulong*)& pControl->configPID.ElevationI;
      conn->Send(&msg);
      break;
    case TYPE_ELEVATION_D:
      pControl->configPID.ElevationD +=vf;
      msg.value = *(ulong*)& pControl->configPID.ElevationD;
      conn->Send(&msg);
      break;

    case TYPE_UNLOCK://解锁电调
      vf = pControl->UnlockMotor();
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      if (vf ) pControl->Flying();
      break;
    case TYPE_SELF_STATIONARY://自稳
      vf =  0;
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_LAND://着陆
      pControl->Landing();
      conn->Send(&msg);
      break;
    case TYPE_INIT_BALANCE://平衡初始化
      vf =  0;
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      pControl->BalanceAdjust();
      break;
    case TYPE_READ_CONFIG://读取配置
      conn->Send(&msg);
      break;
    case TYPE_WRITE_CONFIG://写入配置
      conn->Send(&msg);
      break;

    default:
      break;
    }
  }
}
