#include "delayHandle.h"
#include "pid.h"
#include "ypr.h"
#include "barometer.h"
#include "compass.h"

//最小油门
#define MIN_LEVEL 120
//启动油门
#define ACK_LEVEL 140
//最大油门
#define MAX_LEVEL 150

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

    void SetAllControlPinPwm()
    {
      analogWrite(PIN_FRONT, PinsPwm[FRONT]);
      analogWrite(PIN_AFTER, PinsPwm[AFTER]);
      analogWrite(PIN_LEFT,  PinsPwm[LEFT]);
      analogWrite(PIN_RIGHT, PinsPwm[RIGHT]);
    }

    void SetPins()
    {
      for (int i = 0; i < 4; i++)
      {
        if (PinsPwm[i] > MAX_LEVEL) PinsPwm[i] = MAX_LEVEL;
        if (PinsPwm[i] < ACK_LEVEL) PinsPwm[i] = ACK_LEVEL;
      }
      SetAllControlPinPwm();
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
    void Adjust()
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
          p->SetAllValue(MIN_LEVEL);
          Serial.println("low level motor");
        });
        pUnlockMotorDelay->AddHandle(2000, [](void * pUser) {
          Ccontrol* p = (Ccontrol*) pUser;
          p->SetAllValue(ACK_LEVEL);
          Serial.println("normal level motor");
        });
        pUnlockMotorDelay->AddHandle(4000, [](void * pUser) {
          Ccontrol* p = (Ccontrol*) pUser;
          p->SetAllValue(ACK_LEVEL - 3);
          p->b_unlockMotor = true;
          delete p->pUnlockMotorDelay;
          p->pUnlockMotorDelay = 0;
          Serial.println("unlocking motor finish\n");
        });
      }
    }

    void Start()
    {
      if (!b_unlockMotor) return;
      b_Starting = true;
      SetAllValue(ACK_LEVEL);
      Serial.println("Start");
    }

    void Stop()
    {
      b_Starting = false;
      SetAllValue(MIN_LEVEL);
      Serial.println("Stop");
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
      PinsPwm[LEFT] = ACK_LEVEL - v;
      PinsPwm[RIGHT] = ACK_LEVEL + v;
      SetPins();
    }

    void SetYaw()
    {
      float v = m_compass->GetPoint();
      PinsPwm[FRONT] = ACK_LEVEL - v;
      PinsPwm[AFTER] = ACK_LEVEL - v;
      PinsPwm[LEFT] = ACK_LEVEL + v;
      PinsPwm[RIGHT] = ACK_LEVEL + v;
      SetPins();
    }

    void SetElevation()
    {
      float v = m_barometer->GetPoint();
      PinsPwm[FRONT] = ACK_LEVEL + v;
      PinsPwm[AFTER] = ACK_LEVEL + v;
      PinsPwm[LEFT] = ACK_LEVEL + v;
      PinsPwm[RIGHT] = ACK_LEVEL + v;
      SetPins();
    }

    void SetAllValue(int v)
    {
      PinsPwm[FRONT] = v;
      PinsPwm[AFTER] = v;
      PinsPwm[LEFT]  = v;
      PinsPwm[RIGHT] = v;
      SetAllControlPinPwm();
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
