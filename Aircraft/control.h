#include "delayHandle.h"
#include "pid.h"
#include "ypr.h"
#include "barometer.h"
#include "compass.h"

#define MIN_SERVO 120
#define ACK_SERVO 140
#define MAX_SERVO 250

#define MIN_SPEED 0
#define MAX_SPEED 10000

class Ccontrol
{
private:
  int PIN_FRONT;
  int PIN_AFTER;
  int PIN_LEFT;
  int PIN_RIGHT;

  float PinsPwm[4];
  const int FRONT = 0;
  const int AFTER = 1;
  const int LEFT = 2;
  const int RIGHT = 3;
  bool b_unlockMotor;
  bool b_Starting = false;
  CdelayHandle *pUnlockMotorDelay = NULL;
  Cypr *m_ypr = NULL;
  Cbarometer * m_barometer = NULL;
  Ccompass * m_compass = NULL;
  Cpid *m_PitchPID = NULL;
  Cpid *m_RollPID = NULL;
  Cpid *m_YawPID = NULL;
  Cpid *m_ElevationPID = NULL;

  void WriteAllControlPwmPin(int f, int a, int l, int r)
  {
    analogWrite(PIN_FRONT, f);
    analogWrite(PIN_AFTER, a);
    analogWrite(PIN_LEFT, l);
    analogWrite(PIN_RIGHT, r);
  }

  void SetPins()
  {
    for (int i = 0; i < 4; i++)
    {
      if (PinsPwm[i] > MAX_SPEED) PinsPwm[i] = MAX_SPEED;
      if (PinsPwm[i] < MIN_SPEED) PinsPwm[i] = MIN_SPEED;
    }
    WriteAllControlPwmPin(
    map(PinsPwm[FRONT], MIN_SPEED, MAX_SPEED, ACK_SERVO, MAX_SERVO),
    map(PinsPwm[AFTER], MIN_SPEED, MAX_SPEED, ACK_SERVO, MAX_SERVO),
    map(PinsPwm[LEFT], MIN_SPEED, MAX_SPEED, ACK_SERVO, MAX_SERVO),
    map(PinsPwm[RIGHT], MIN_SPEED, MAX_SPEED, ACK_SERVO, MAX_SERVO));
  }

public:
  Ccontrol(int pinFront, int pinAfter, int pinLeft, int pinRight)
  {
    PIN_FRONT = pinFront;
    PIN_AFTER = pinAfter;
    PIN_LEFT = pinLeft;
    PIN_RIGHT = pinRight;
    m_ypr = new Cypr();
    m_barometer = new Cbarometer();
    m_compass =  new Ccompass();
    m_PitchPID =  new Cpid(0, 0, 0, 0);
    m_RollPID =  new Cpid(0, 0, 0, 0);
    m_YawPID =  new Cpid(0, 0, 0, 0);
    m_ElevationPID =  new Cpid(0, 0, 0, 0);
  }

  ~Ccontrol()
  {
  }

  void Init()
  {
    pinMode(PIN_FRONT, OUTPUT);
    pinMode(PIN_AFTER, OUTPUT);
    pinMode(PIN_LEFT, OUTPUT);
    pinMode(PIN_RIGHT, OUTPUT);
    m_ypr->Init();
  }

  //校准平衡数据
  void Adjust(double pitch, double roll, double yaw, double elevation)
  {
  }

  bool UnlockMotor()
  {
    if (b_Starting) return false;
    if (!pUnlockMotorDelay)
    {
      pUnlockMotorDelay = new CdelayHandle((void*)this);
      pUnlockMotorDelay->AddHandle(0, [](void * pUser) {
        Ccontrol* p = (Ccontrol*) pUser;
        p->SetAllValue(MIN_SERVO);
        Serial.println("low level motor");
      }
      );
      pUnlockMotorDelay->AddHandle(2000, [](void * pUser) {
        Ccontrol* p = (Ccontrol*) pUser;
        p->SetAllValue(ACK_SERVO);
        Serial.println("normal level motor");
      }
      );
      pUnlockMotorDelay->AddHandle(4000, [](void * pUser) {
        Ccontrol* p = (Ccontrol*) pUser;
        p->SetAllValue(ACK_SERVO - 3);
        p->b_unlockMotor = true;
        delete p->pUnlockMotorDelay;
        p->pUnlockMotorDelay = 0;
        Serial.println("unlocking motor finish\n");
      }
      );
    }
  }

  void Start()
  {
    if (!b_unlockMotor) return;
    b_Starting = true;
    SetAllValue(ACK_SERVO);
    Serial.println("Start");
  }

  void Stop()
  {
    b_Starting = false;
    SetAllValue(MIN_SERVO);
    Serial.println("Stop");
  }

  void SetAllValue(int v)
  {
    WriteAllControlPwmPin(v, v, v, v);
  }

  void SetPitch()
  {
    float v = m_ypr->GetPitchPoint();
    Serial.print("pitch:  ");
    Serial.print(v);
    v = m_PitchPID->IncPIDCalc(v);
    Serial.print("  pitchPID:  ");
    Serial.print(v);
    PinsPwm[FRONT] -= (float)v/1000;
    PinsPwm[AFTER] += (float)v/1000;
    SetPins();
  }

  void SetPitch(float pp, float ii, float dd, float tt)
  {
    m_PitchPID->ReSetPID(pp, ii, dd);
    m_PitchPID->ReSetPoint(tt);
  }

  void SetRoll()
  {
    float v = m_ypr->GetPitchPoint();
    PinsPwm[LEFT] -= v;
    PinsPwm[RIGHT] += v;
    SetPins();
  }

  void SetYaw()
  {
    float v = m_compass->GetPoint();
    PinsPwm[FRONT] -= v;
    PinsPwm[AFTER] -= v;
    PinsPwm[LEFT] += v;
    PinsPwm[RIGHT] += v;
    SetPins();
  }

  void SetElevation()
  {
    float v = m_barometer->GetPoint();
    PinsPwm[FRONT] += v;
    PinsPwm[AFTER] += v;
    PinsPwm[LEFT] += v;
    PinsPwm[RIGHT] += v;
    SetPins();
  }

  void Motor()
  {
    if (pUnlockMotorDelay) pUnlockMotorDelay->Handle();

    if (b_Starting)
    {
      SetPitch();
    }
  }
};
