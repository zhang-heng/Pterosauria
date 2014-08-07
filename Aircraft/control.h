#include "delayHandle.h"
#include "pid.h"
#include "ypr.h"
#include "barometer.h"
#include "compass.h"

//最小油门
#define MIN_LEVEL 120
//启动油门
#define ACK_LEVEL 132
//最大油门
#define MAX_LEVEL 200

class Ccontrol
{
private:
  int PIN_FRONT;
  int PIN_AFTER;
  int PIN_LEFT;
  int PIN_RIGHT;

  int PinsPwm[4];
  const int FRONT = 0;
  const int AFTER = 1;
  const int LEFT = 2;
  const int RIGHT = 3;
  bool b_unlockMotor;
  CdelayHandle *pUnlockMotorDelay = NULL;
  Cypr *m_ypr;
  Cbarometer * m_barometer;
  Ccompass * m_compass;
  Cpid *m_pid;

  void SetAllControlPinPwm()
  {
    analogWrite(PIN_FRONT, PinsPwm[FRONT]);
    analogWrite(PIN_AFTER, PinsPwm[AFTER]);
    analogWrite(PIN_LEFT,  PinsPwm[LEFT]);
    analogWrite(PIN_RIGHT, PinsPwm[RIGHT]); 
  }

  void SetPins()
  {
    for (int i =0;i<4;i++)
    {
      if (PinsPwm[i]>MAX_LEVEL) PinsPwm[i] = MAX_LEVEL;
      if (PinsPwm[i]<ACK_LEVEL) PinsPwm[i] = ACK_LEVEL;
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
    m_pid =  new Cpid(0, 0, 0, 0);
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

  void Motor()
  {
  }

  bool UnlockMotor()
  {
    if (!pUnlockMotorDelay) 
    {
      pUnlockMotorDelay = new CdelayHandle((void*)this);
      pUnlockMotorDelay->AddHandle(0, [](void * pUser) { 
        Ccontrol* p = (Ccontrol*) pUser;  
        p->SetAllValue(ACK_LEVEL); 
      } 
      );
      pUnlockMotorDelay->AddHandle(1000, [](void * pUser) { 
        Ccontrol* p = (Ccontrol*) pUser;  
        p->SetAllValue(MIN_LEVEL); 
      } 
      );
      pUnlockMotorDelay->AddHandle(3000, [](void * pUser) { 
        Ccontrol* p = (Ccontrol*) pUser; 
        p->SetAllValue(ACK_LEVEL); 
      } 
      );
      pUnlockMotorDelay->AddHandle(4000, [](void * pUser) {
        Ccontrol* p = (Ccontrol*) pUser;  
        p->SetAllValue(ACK_LEVEL - 3); 
        p->b_unlockMotor = true;
      } 
      );
    }
    pUnlockMotorDelay->Handle();
  }

  void Start()
  {
    SetAllValue(ACK_LEVEL);
  }

  void Stop()
  {
    SetAllValue(ACK_LEVEL - 2);
  }

  void SetPitch()
  {
    float v = m_ypr->GetPitchPoint();
    PinsPwm[FRONT] = ACK_LEVEL - v;
    PinsPwm[AFTER] = ACK_LEVEL + v;
    SetPins();
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
};


















































