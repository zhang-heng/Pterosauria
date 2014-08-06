#include "Arduino.h"

//四轴管脚定义
#define PIN_FRONT 10
#define PIN_AFTER 3
#define PIN_LEFT  9
#define PIN_RIGHT 11
//当前的四轴油门值
#define SET_FRONT 0
#define SET_AFTER 1
#define SET_LEFT  2
#define SET_RIGHT 4
static int PinsPwm[4];
//ypr初始化值，用于校正平衡姿态
#define CORRENT_YAW   0
#define CORRENT_PITCH 1
#define CORRENT_ROLL  2
static int CorrectYPR[3];
//ypr目标值
#define CONTROL_YAW   0
#define CONTROL_PITCH 1
#define CONTROL_ROLL  2
static int ControlYPR[3];
//油门级别
static int LevelPwm = 0;
//横向旋转速率
static int LevelYaw = 0;

//最小启动油门
#define MIN_LEVEL 130

void SetYaw(int v) //C
{
  LevelYaw - v; 
  analogWrite(SET_FRONT,PinsPwm[SET_FRONT]);
  analogWrite(SET_AFTER,PinsPwm[SET_AFTER]);  
}

void SetPitch(int v) //->
{
  PinsPwm[SET_FRONT] = LevelPwm - v/2;
  PinsPwm[SET_AFTER] = LevelPwm + v/2;
  analogWrite(PIN_FRONT,PinsPwm[SET_FRONT]);
  analogWrite(PIN_AFTER,PinsPwm[SET_AFTER]);
}

void SetRoll(int v) //T
{
  PinsPwm[SET_LEFT] = LevelPwm;
  PinsPwm[SET_RIGHT] = LevelPwm;
  if (v<0) PinsPwm[SET_LEFT] = LevelPwm;
  if (v<0) PinsPwm[SET_RIGHT] = LevelPwm;
  analogWrite(SET_LEFT,PinsPwm[SET_LEFT]);
  analogWrite(SET_RIGHT,PinsPwm[SET_RIGHT]);
}

void SetAllPinPwm()
{
  analogWrite(PIN_FRONT, PinsPwm[SET_FRONT]);
  analogWrite(PIN_AFTER, PinsPwm[SET_AFTER]);
  analogWrite(PIN_LEFT,  PinsPwm[SET_LEFT]);
  analogWrite(PIN_RIGHT, PinsPwm[SET_RIGHT]);  
}

void SetAllValue(int v)
{
  PinsPwm[SET_FRONT] = v;
  PinsPwm[SET_AFTER] = v;
  PinsPwm[SET_LEFT]  = v;
  PinsPwm[SET_RIGHT] = v;
  SetAllPinPwm();
}
