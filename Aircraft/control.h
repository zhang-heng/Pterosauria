#include "pid.h"
#include "ypr.h"
#include "barometer.h"
#include "compass.h"
#include "config.h"
#include "sonar.h"

#define UNLOCK_SERVO 700//停机
#define MAX_SERVO 2000//最大速率

#define FRONT 0
#define AFTER 1
#define LEFT 2
#define RIGHT 3

#define PITCH 0
#define ROLL 1
#define YAW 2
#define ELEVATION 3

class Ccontrol
{
 private:
  Cypr m_ypr;              //读取MPU6050值,此陀螺仪有效的是倾斜值
  Cbarometer m_barometer;  //读取气压,暂时不用,精度太差
  Ccompass m_compass;      //读取HMC5883L值,此磁感获取航向
  Csonar m_sonar();        //声呐获取对地高度,相对气压值精准

  //正在着陆
  bool m_landing;
  //
  bool m_flying;
  //推力?油门?
  float m_power;
  //姿态调整时间
  ulong m_lastOptTime;

 public:
  //姿态
  //配置
  StructConfig configPID;
  //四轴电调频率
  float ServosValue[4];
  //姿态目标
  float Targets[4];
  //PID
  Cpid *m_PIDs[4];

  //电调
  Servo Servos[4];
  //电调管脚
  int ServoPins[4];
  //每个电机的具体值有偏差
  int ServoMin[4];

  Ccontrol(){
    ServoPins[0] = 5;
    ServoPins[1] = 9;
    ServoPins[2] = 6;
    ServoPins[3] = 10;
    ServoMin[0] = 1060;
    ServoMin[1] = 1060;
    ServoMin[2] = 810;
    ServoMin[3] = 1020;
    for(int i =0;i<4;i++){
      Servos[i].attach(ServoPins[i]);
      ServosValue[i] = UNLOCK_SERVO;
      Servos[i].writeMicroseconds(UNLOCK_SERVO);
      m_PIDs[i] = new Cpid (0,0,0,0);
      Targets[i] = 0;
    }
    m_power = 0;
    m_lastOptTime = 0;
    m_ypr.Init();
    //获取rom配置
    ReadRom(&configPID);
  }

  ///////////////////刷新各传感器数据///////////////////////
  float GetPitch(){
    return m_ypr.GetPitchPoint() - configPID.AdjustPitch;
  }
  float GetRoll(){
    return -(m_ypr.GetRollPoint() - configPID.AdjustRoll);
  }
  float GetYaw(){
    return  m_compass.GetPoint();
  }
  float GetElevation(){
    return m_power;//m_compass.GetPoint() - configPID.AdjustElevation;
  }

  void FlushSensors(){
    GetPitch();
    GetRoll();
    GetYaw();
    GetElevation();
  }

  //姿态修改
  void MotionPitch(float v){
    Targets[PITCH] = map (v, -100, 100, -15, 15);//映射为15度调整
  }

  void MotionRoll(float v){
    Targets[ROLL] = map (v, -100, 100, -15, 15);//映射为15度调整
  }

  void MotionElevation(float v){
    m_power += v/100;
    if (m_power<0) m_power = 0;
  }

  void MotionYaw(float v){
    Targets[YAW] = GetYaw() + v/10;
    if (Targets[YAW]<0) Targets[YAW] += 360;
    if (Targets[YAW]>360) Targets[YAW] -= 360;
  }

  //飞行处理,姿态平衡
  void Flying(){
    ulong currentTime = millis();
    if(currentTime - m_lastOptTime > 20){
      m_lastOptTime = currentTime;
      OptPitch();
      OptRoll();
      OptYaw();
      WriteAllServos();
      //OptElevation();
    }
  }

  //自稳
  void SelfStationary(){
    Targets[PITCH] = 0;
    Targets[ROLL] = 0;
    Flying();
  }

  //着陆处理
  void Landing(){
    m_power = 0;
    Targets[PITCH] = 0;
    Targets[ROLL] = 0;
    for(int i =0;i<4;i++){
      ServosValue[i] = 0;
      Servos[i].writeMicroseconds(UNLOCK_SERVO);
    }
  }

  //配置写入rom
  void SaveConfig(){
    WriteRom(&configPID);
  }

  //校准传感器
  void BalanceAdjust(){
    float p = m_ypr.GetPitchPoint();
    float r = m_ypr.GetRollPoint();
    float y = m_compass.GetPoint();
    for (int i = 0 ; i < 20; i++){
      p = (p + m_ypr.GetPitchPoint()) / 2;
      r = (r + m_ypr.GetRollPoint()) / 2;
      y = (y + m_compass.GetPoint()) / 2;
      delay(20);
    }
    configPID.AdjustPitch = p;
    configPID.AdjustRoll = r;
    configPID.AdjustYaw = y;
  }

  //计算调整值
  void OptPitch() {
    m_PIDs[PITCH]->ReSetPID(configPID.PitchP, configPID.PitchI, configPID.PitchD);
    m_PIDs[PITCH]->ReSetPoint(Targets[PITCH]);
    float v = GetPitch();
    v = m_PIDs[PITCH]->PIDCalc(v);
    ServosValue[FRONT] = m_power - v;
    ServosValue[AFTER] = m_power + v;
  }

  void OptRoll() {
    m_PIDs[ROLL]->ReSetPID(configPID.RollP, configPID.RollI, configPID.RollD);
    m_PIDs[ROLL]->ReSetPoint(Targets[ROLL]);
    float v = GetRoll();
    v = m_PIDs[ROLL]->PIDCalc(v);
    ServosValue[LEFT] = m_power + v;
    ServosValue[RIGHT] = m_power - v;
  }

  void OptYaw() {
    m_PIDs[YAW]->ReSetPID(configPID.YawP, configPID.YawI, configPID.YawD);
    m_PIDs[YAW]->ReSetPoint(Targets[YAW] - 180.00);
    float v = GetYaw() - 180;
    v = m_PIDs[YAW]->PIDCalc(v);
    ServosValue[FRONT] += v;
    ServosValue[AFTER] += v;
    ServosValue[LEFT] -= v;
    ServosValue[RIGHT] -= v;
  }

  void OptElevation() {
    m_PIDs[ELEVATION]->ReSetPID(configPID.ElevationP, configPID.ElevationI, configPID.ElevationD);
    m_PIDs[ELEVATION]->ReSetPoint(Targets[ELEVATION]);
    float v = GetElevation();
    v = m_PIDs[ELEVATION]->PIDCalc(v);
    ServosValue[FRONT] += v;
    ServosValue[AFTER] += v;
    ServosValue[LEFT] += v;
    ServosValue[RIGHT] += v;
  }

  void WriteAllServos(){
    for(int i =0;i<4;i++){
      if(ServosValue[i] < 0) ServosValue[i] = 0;
      if(ServosValue[i] > m_power + 100) ServosValue[i] = m_power + 100;//力度阈限制
      if(ServosValue[i] > MAX_SERVO - ServoMin[i]) ServosValue[i] = MAX_SERVO - ServoMin[i];
      Servos[i].writeMicroseconds(ServosValue[i] + ServoMin[i]);
    }
  }
};
