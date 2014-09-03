#include "delayHandle.h"
#include "pid.h"
#include "ypr.h"
#include "barometer.h"
#include "compass.h"
#include "config.h"
#include "sonar.h"

//低频率解锁电调
#define MIN_SERVO 110
//启动频率 实际测试pwm:131-132就能启动
#define ACK_SERVO 140
//最大pwm值
#define MAX_SERVO 250

//用于参与pid计算的速度值,最终映射到servo值,输出给电调
#define MIN_SPEED 0
#define MAX_SPEED 3000

class Ccontrol
{
 public:
  StructConfig configPID;
  Ccontrol(int pinFront, int pinAfter, int pinLeft, int pinRight){
    //四轴四个方向的管脚
    PIN_FRONT = pinFront;
    PIN_AFTER = pinAfter;
    PIN_LEFT = pinLeft;
    PIN_RIGHT = pinRight;
    pinMode(PIN_FRONT, OUTPUT);
    pinMode(PIN_AFTER, OUTPUT);
    pinMode(PIN_LEFT, OUTPUT);
    pinMode(PIN_RIGHT, OUTPUT);

    //读取MPU6050值,此陀螺仪有效的是倾斜值
    m_ypr = new Cypr();
    m_ypr->Init();
    //读取HMC5883L值,此磁感获取航向
    m_compass =  new Ccompass();
    //读取气压,暂时不用,精度太差.
    m_barometer = new Cbarometer();
    //声呐获取对地高度,相对气压值精准
    m_sonar = new Csonar(1,2,3);

    //获取rom配置
    ReadRom(&configPID);
    //初始化四参的pid
    m_PitchPID =  new Cpid(configPID.PitchP, configPID.PitchI, configPID.PitchD, 0);
    m_RollPID =  new Cpid(configPID.RollP, configPID.RollI, configPID.RollD, 0);
    m_YawPID =  new Cpid(configPID.YawP, configPID.YawI, configPID.YawP, 0);
    m_ElevationPID =  new Cpid(configPID.ElevationP, configPID.ElevationI, configPID.ElevationD, 0);
  }

  ///////////////////刷新各传感器数据///////////////////////
  float GetPitch(){
    return m_ypr->GetPitchPoint() - configPID.BalancePitch;
  }
  float GetRoll(){
    return m_ypr->GetRollPoint() - configPID.BalanceRoll;
  }
  float GetYaw(){
    return m_compass->GetPoint() - configPID.BalanceYaw;
  }
  float GetElevation(){
    return m_compass->GetPoint() - configPID.BalanceYaw;
  }

  //飞行处理
  void Flying(){
    WriteAllControlPwmPin(Speeds[0],Speeds[1],Speeds[2],Speeds[3]);
  }

  //着陆处理
  void Landing(){
  }

  //配置写入rom
  void SaveConfig(){
    WriteRom(&configPID);
  }

  //解锁电调
  bool UnlockMotor(){
    if (b_unlockMotor) return true;
    if (!pUnlockMotorDelay){
      pUnlockMotorDelay = new CdelayHandle((void*)this);
      pUnlockMotorDelay->AddHandle(0, [](void * pUser){
          Ccontrol* p = (Ccontrol*) pUser;
          p->SetAllValue(MIN_SERVO);
          Serial.println("low level motor");});

      pUnlockMotorDelay->AddHandle(2000, [](void * pUser){
          Ccontrol* p = (Ccontrol*) pUser;
          p->SetAllValue(ACK_SERVO);
          Serial.println("normal level motor");});

      pUnlockMotorDelay->AddHandle(4000, [](void * pUser){
          Ccontrol* p = (Ccontrol*) pUser;
          p->SetAllValue(MIN_SERVO);
          p->b_unlockMotor = true;
          delete p->pUnlockMotorDelay;
          //p->pUnlockMotorDelay = 0;
          Serial.println("unlocking motor finish\n");});}
    pUnlockMotorDelay->Handle();
    return false;
  }

  //获取传感器校准
  void BalanceAdjust(){
    float p = m_ypr->GetPitchPoint();
    float r = m_ypr->GetRollPoint();
    float y = m_compass->GetPoint();
    for (int i = 0 ; i < 100; i++){
      p = (p + m_ypr->GetPitchPoint()) / 2;
      r = (r + m_ypr->GetRollPoint()) / 2;
      y = (y + m_compass->GetPoint()) / 2;
      delay(20);
    }
    configPID.BalancePitch = p;
    configPID.BalanceRoll = r;
    configPID.BalanceYaw = y;
  }

  void TrimmingTarget(double pitch, double roll, double yaw, double elevation) {

  }




















  void Start() {
    if (!b_unlockMotor) return;
    b_Starting = true;
    for (int i = 0; i < 4; i++)
      Speeds[i] = MIN_SPEED;
    Serial.println("Start");
  }

  void Stop() {
    b_Starting = false;
    SetAllValue(MIN_SERVO);
    Serial.println("Stop");
  }

  void SetAllValue(int v) {
    WriteAllControlPwmPin(v, v, v, v);
  }

  void SetPitch(float v) {
    v = m_PitchPID->IncPIDCalc(v);
    Speeds[FRONT] -= v;
    Speeds[AFTER] += v;
  }

  void SetPitch(float pp, float ii, float dd, float tt) {
    m_PitchPID->ReSetPID(pp, ii, dd);
    m_PitchPID->ReSetPoint(tt);
  }

  void SetRoll(float v) {
    v = m_RollPID->IncPIDCalc(v);
    Speeds[LEFT] -= v;
    Speeds[RIGHT] += v;
  }

  void SetYaw(float v) {
    v = m_YawPID->IncPIDCalc(v);
    Speeds[FRONT] -= v;
    Speeds[AFTER] -= v;
    Speeds[LEFT] += v;
    Speeds[RIGHT] += v;
  }

  void SetElevation() {
    float v = m_barometer->GetPoint();
    Speeds[FRONT] += v;
    Speeds[AFTER] += v;
    Speeds[LEFT] += v;
    Speeds[RIGHT] += v;
    SetPins();
  }

  void AddSpeed() {
    Speeds[FRONT] += 1000;
    Speeds[AFTER] += 1000;
    Speeds[LEFT] += 1000;
    Speeds[RIGHT] += 1000;
  }

  void SubtractSpeed(){
    Speeds[FRONT] -= 1000;
    Speeds[AFTER] -= 1000;
    Speeds[LEFT] -= 1000;
    Speeds[RIGHT] -= 1000;
  }

  void Motor(){
    float p = m_ypr->GetPitchPoint() - configPID.BalancePitch;
    float r = m_ypr->GetRollPoint() - configPID.BalanceRoll;
    float y = m_ypr->GetYawPoint() - configPID.BalanceYaw;
    m_PitchPID->ReSetPID(configPID.PitchP, configPID.PitchI, configPID.PitchD);
    m_RollPID->ReSetPID(configPID.RollP, configPID.RollI, configPID.RollD);
    m_YawPID->ReSetPID(configPID.YawP, configPID.YawI, configPID.YawD);

    if (b_Starting){
      if (abs(p) > 0.2) SetPitch(p);
      if (abs(p) > 0.2) SetRoll(r);
      if (abs(p) > 0.2) SetYaw(y);
      SetPins();
    }
  }

  long Speeds[4];


 private:
  int PIN_FRONT;
  int PIN_AFTER;
  int PIN_LEFT;
  int PIN_RIGHT;

  const int FRONT = 0;
  const int AFTER = 1;
  const int LEFT = 2;
  const int RIGHT = 3;
  bool b_unlockMotor = false;
  bool b_Starting = false;
  CdelayHandle *pUnlockMotorDelay = NULL;
  Cypr *m_ypr = NULL;
  Cbarometer * m_barometer = NULL;
  Ccompass * m_compass = NULL;
  Csonar * m_sonar = NULL;

  Cpid *m_PitchPID = NULL;
  Cpid *m_RollPID = NULL;
  Cpid *m_YawPID = NULL;
  Cpid *m_ElevationPID = NULL;

  void WriteAllControlPwmPin(int f, int a, int l, int r){
    analogWrite(PIN_FRONT, f);
    analogWrite(PIN_AFTER, a);
    analogWrite(PIN_LEFT, l);
    analogWrite(PIN_RIGHT, r);
  }









  void SetPins()
  {
    for (int i = 0; i < 4; i++)
      {
        if (Speeds[i] > MAX_SPEED) Speeds[i] = MAX_SPEED;
        if (Speeds[i] < MIN_SPEED) Speeds[i] = MIN_SPEED;
      }
    WriteAllControlPwmPin(map(Speeds[FRONT], MIN_SPEED, MAX_SPEED, ACK_SERVO, MAX_SERVO),
                          map(Speeds[AFTER], MIN_SPEED, MAX_SPEED, ACK_SERVO, MAX_SERVO),
                          map(Speeds[LEFT], MIN_SPEED, MAX_SPEED, ACK_SERVO, MAX_SERVO),
                          map(Speeds[RIGHT], MIN_SPEED, MAX_SPEED, ACK_SERVO, MAX_SERVO));
  }
};
