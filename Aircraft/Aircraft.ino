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
  pPower = new Cpower(A2);
  Serial.println("Initializing finish");
}

//网络接收数据包
NetStruct msg;
ulong lastRecvTime =0;

//循环体
void loop()
{
  //刷新传感器
  pControl->FlushSensors();
  ulong currentTime = millis();

  //循环过程中频繁访问网络,以免遥控端延迟丢包.
  for (int i = 0; i < 10; i++){
    conn->Read(&msg);

    if(msg.type != TYPE_UNKNOW) lastRecvTime = currentTime;

    float vf = 0;
    int vi = 0;
    switch (msg.type){
    case TYPE_TIME_SYN://获取飞控运行时间
      msg.value = *(ulong*)& currentTime;
      conn->Send(&msg);
      break;
    case TYPE_POWER://获取电池百分比
      vf = pPower->GetVoltage();
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;

    case TYPE_UNLOCK_FLY://解锁电调处理飞行
      vf = pControl->UnlockMotor();
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_SELF_STATIONARY://自稳
      pControl->SelfStationary();
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

      //姿态调整
    case TYPE_PITCH://调整/获取俯仰状态
      pControl->MotionPitch(*(int*)msg.value);
      vf =  pControl->GetPitch();
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_ROLL://调整/获取横滚状态
      pControl->MotionRoll(*(int*)msg.value);
      vf = pControl->GetRoll();
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_YAW://调整/获取航向状态
      pControl->MotionYaw(*(int*)msg.value);
      vf = pControl->GetYaw();
      msg.value = *(ulong*)& vf;
      conn->Send(&msg);
      break;
    case TYPE_ELEVATION://调整/获取高度状态
      pControl->MotionElevation(*(int*)msg.value);
      conn->Send(&msg);
      break;

      /*************调试参数************/
    case TYPE_READ_CONFIG://读取配置
      conn->Send(&msg);
      break;
    case TYPE_WRITE_CONFIG://写入配置
      conn->Send(&msg);
      break;

    case TYPE_MOTOR_A://调整/获取A电调频率
      pControl->Servos[0] += *(int*)&msg.value;
      msg.value = *(ulong*)&pControl->Servos[0];
      conn->Send(&msg);
      break;
    case TYPE_MOTOR_B://调整/获取B电调频率
      pControl->Servos[1] += *(int*)&msg.value;
      msg.value = *(ulong*)&pControl->Servos[1];
      conn->Send(&msg);
      break;
    case TYPE_MOTOR_C://调整/获取C电调频率
      pControl->Servos[2] += *(int*)&msg.value;
      msg.value = *(ulong*)&pControl->Servos[2];
      conn->Send(&msg);
      break;
    case TYPE_MOTOR_D://调整/获取D电调频率
      pControl->Servos[3] += *(int*)&msg.value;
      msg.value = *(ulong*)&pControl->Servos[3];
      conn->Send(&msg);
      break;

    case TYPE_PITCH_P:
      pControl->configPID.PitchP += *(float*)&msg.value;
      msg.value = *(ulong*)& pControl->configPID.PitchP;
      conn->Send(&msg);
      break;
    case TYPE_PITCH_I:
      pControl->configPID.PitchI += *(float*)&msg.value;
      msg.value = *(ulong*)& pControl->configPID.PitchI;
      conn->Send(&msg);
      break;
    case TYPE_PITCH_D:
      pControl->configPID.PitchD += *(float*)&msg.value;
      msg.value = *(ulong*)& pControl->configPID.PitchD;
      conn->Send(&msg);
      break;
    case TYPE_ROLL_P:
      pControl->configPID.RollP += *(float*)&msg.value;
      msg.value = *(ulong*)& pControl->configPID.RollP;
      conn->Send(&msg);
      break;
    case TYPE_ROLL_I:
      pControl->configPID.RollI += *(float*)&msg.value;
      msg.value = *(ulong*)& pControl->configPID.RollI;
      conn->Send(&msg);
      break;
    case TYPE_ROLL_D:
      pControl->configPID.RollD += *(float*)&msg.value;
      msg.value = *(ulong*)& pControl->configPID.RollD;
      conn->Send(&msg);
      break;
    case TYPE_YAW_P:
      pControl->configPID.YawP += *(float*)&msg.value;
      msg.value = *(ulong*)& pControl->configPID.YawP;
      conn->Send(&msg);
      break;
    case TYPE_YAW_I:
      pControl->configPID.YawI += *(float*)&msg.value;
      msg.value = *(ulong*)& pControl->configPID.YawI;
      conn->Send(&msg);
      break;
    case TYPE_YAW_D:
      pControl->configPID.YawD += *(float*)&msg.value;
      msg.value = *(ulong*)& pControl->configPID.YawD;
      conn->Send(&msg);
      break;
    case TYPE_ELEVATION_P:
      pControl->configPID.ElevationP += *(float*)&msg.value;
      msg.value = *(ulong*)& pControl->configPID.ElevationP;
      conn->Send(&msg);
      break;
    case TYPE_ELEVATION_I:
      pControl->configPID.ElevationI += *(float*)&msg.value;
      msg.value = *(ulong*)& pControl->configPID.ElevationI;
      conn->Send(&msg);
      break;
    case TYPE_ELEVATION_D:
      pControl->configPID.ElevationD += *(float*)&msg.value;
      msg.value = *(ulong*)& pControl->configPID.ElevationD;
      conn->Send(&msg);
      break;

    default:
      break;
    }
  }

  //1s未收到数据,在飞行状态则进入悬停.
  if (currentTime - lastRecvTime > 1000){
    pControl->SelfStationary();
  }
}
