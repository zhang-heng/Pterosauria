#include "delayHandle.h"
#include "pid.h"
#include "ypr.h"
#include "barometer.h"
#include "compass.h"
#include "config.h"

#define MIN_SERVO 120
#define ACK_SERVO 140
#define MAX_SERVO 250

#define MIN_SPEED 0
#define MAX_SPEED 3000

class Ccontrol
{
  public:
    StructConfig configPID;
    Ccontrol(int pinFront, int pinAfter, int pinLeft, int pinRight)
    {
      PIN_FRONT = pinFront;
      PIN_AFTER = pinAfter;
      PIN_LEFT = pinLeft;
      PIN_RIGHT = pinRight;
      m_ypr = new Cypr();
      m_barometer = new Cbarometer();
      m_compass =  new Ccompass();

      pinMode(PIN_FRONT, OUTPUT);
      pinMode(PIN_AFTER, OUTPUT);
      pinMode(PIN_LEFT, OUTPUT);
      pinMode(PIN_RIGHT, OUTPUT);
      ReadRom(&configPID);
      m_PitchPID =  new Cpid(configPID.PitchP, configPID.PitchI, configPID.PitchD, 0);
      m_RollPID =  new Cpid(configPID.RollP, configPID.RollI, configPID.RollD, 0);
      m_YawPID =  new Cpid(configPID.YawP, configPID.YawI, configPID.YawP, 0);
      m_ElevationPID =  new Cpid(configPID.ElevationP, configPID.ElevationI, configPID.ElevationD, 0);
      m_ypr->Init();
    }

    float GetPitch()  {
      return m_ypr->GetPitchPoint() - configPID.BalancePitch;
    }

    float GetRoll()  {
      return m_ypr->GetRollPoint() - configPID.BalanceRoll;
    }

    float GetYaw()  {
      return m_compass->GetPoint() - configPID.BalanceYaw;
    }


    void SaveConfig()
    {
      WriteRom(&configPID);
    }

    void BalanceAdjust()
    {
      float p = m_ypr->GetPitchPoint();
      float r = m_ypr->GetRollPoint();
      float y = m_compass->GetPoint();
      for (int i = 0 ; i < 100; i++)
      {
        p = (p + m_ypr->GetPitchPoint()) / 2;
        r = (r + m_ypr->GetRollPoint()) / 2;
        y = (y + m_compass->GetPoint()) / 2;
        delay(10);
      }
      configPID.BalancePitch = p;
      configPID.BalanceRoll = r;
      configPID.BalanceYaw = y;
    }

    void TrimmingTarget(double pitch, double roll, double yaw, double elevation)
    {

    }

    bool UnlockMotor()
    {
      if (b_unlockMotor) return true;
      if (!pUnlockMotorDelay)
      {
        pUnlockMotorDelay = new CdelayHandle((void*)this);
        pUnlockMotorDelay->AddHandle(0, [](void * pUser) {
          Ccontrol* p = (Ccontrol*) pUser;
          p->SetAllValue(MIN_SERVO);
          Serial.println("low level motor");
        });
        pUnlockMotorDelay->AddHandle(2000, [](void * pUser) {
          Ccontrol* p = (Ccontrol*) pUser;
          p->SetAllValue(ACK_SERVO);
          Serial.println("normal level motor");
        });
        pUnlockMotorDelay->AddHandle(4000, [](void * pUser) {
          Ccontrol* p = (Ccontrol*) pUser;
          p->SetAllValue(MIN_SERVO);
          p->b_unlockMotor = true;
          delete p->pUnlockMotorDelay;
          //p->pUnlockMotorDelay = 0;
          Serial.println("unlocking motor finish\n");
        });
      }
      pUnlockMotorDelay->Handle();
      return false;
    }

    void Start()
    {
      if (!b_unlockMotor) return;
      b_Starting = true;
      for (int i = 0; i < 4; i++)
        Speeds[i] = MIN_SPEED;
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

    void SetPitch(float v)
    {
      v = m_PitchPID->IncPIDCalc(v);
      Speeds[FRONT] -= v;
      Speeds[AFTER] += v;
    }

    void SetPitch(float pp, float ii, float dd, float tt)
    {
      m_PitchPID->ReSetPID(pp, ii, dd);
      m_PitchPID->ReSetPoint(tt);
    }

    void SetRoll(float v)
    {
      v = m_RollPID->IncPIDCalc(v);
      Speeds[LEFT] -= v;
      Speeds[RIGHT] += v;
    }

    void SetYaw(float v)
    {
      v = m_YawPID->IncPIDCalc(v);
      Speeds[FRONT] -= v;
      Speeds[AFTER] -= v;
      Speeds[LEFT] += v;
      Speeds[RIGHT] += v;
    }

    void SetElevation()
    {
      float v = m_barometer->GetPoint();
      Speeds[FRONT] += v;
      Speeds[AFTER] += v;
      Speeds[LEFT] += v;
      Speeds[RIGHT] += v;
      SetPins();
    }

    void AddSpeed()
    {
      Speeds[FRONT] += 1000;
      Speeds[AFTER] += 1000;
      Speeds[LEFT] += 1000;
      Speeds[RIGHT] += 1000;
    }

    void SubtractSpeed()
    {
      Speeds[FRONT] -= 1000;
      Speeds[AFTER] -= 1000;
      Speeds[LEFT] -= 1000;
      Speeds[RIGHT] -= 1000;
    }

    void Motor()
    {
      float p = m_ypr->GetPitchPoint() - configPID.BalancePitch;
      float r = m_ypr->GetRollPoint() - configPID.BalanceRoll;
      float y = m_ypr->GetYawPoint() - configPID.BalanceYaw;
      m_PitchPID->ReSetPID(configPID.PitchP, configPID.PitchI, configPID.PitchD);
      m_RollPID->ReSetPID(configPID.RollP, configPID.RollI, configPID.RollD);
      m_YawPID->ReSetPID(configPID.YawP, configPID.YawI, configPID.YawD);

      if (b_Starting)
      {
        if (abs(p) > 0.2) SetPitch(p);
        if (abs(p) > 0.2) SetRoll(r);
        if (abs(p) > 0.2) SetYaw(y);
        SetPins();
      }
    }

  private:
    int PIN_FRONT;
    int PIN_AFTER;
    int PIN_LEFT;
    int PIN_RIGHT;

    long Speeds[4];
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
      if (f > 210) f = 210;
      if (a > 210) a = 210;
      if (l > 210) l = 210;
      if (r > 210) r = 210;
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
      WriteAllControlPwmPin(
        map(Speeds[FRONT], MIN_SPEED, MAX_SPEED, ACK_SERVO, MAX_SERVO),
        map(Speeds[AFTER], MIN_SPEED, MAX_SPEED, ACK_SERVO, MAX_SERVO),
        map(Speeds[LEFT], MIN_SPEED, MAX_SPEED, ACK_SERVO, MAX_SERVO),
        map(Speeds[RIGHT], MIN_SPEED, MAX_SPEED, ACK_SERVO, MAX_SERVO));
    }
};
