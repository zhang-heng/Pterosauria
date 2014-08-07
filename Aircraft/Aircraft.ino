#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Wire.h>

#include "control.h"

double pp = 0;
double ii = 0;
double dd = 0;

Ccontrol * pControl;

void setup()
{
  Serial.begin(115200);
  while (Serial.available() && Serial.read());
  if (pControl) delete pControl;
  pControl =  new Ccontrol(10, 3, 9, 11);
  pControl->Init();
}

void loop()
{
  pControl-> Motor();
  pControl->SetPitch(pp,ii,dd,0);
  int val = Serial.read();
  if (val == 's')
    pControl->UnlockMotor();
  else if (val == '1')
    pControl->Start();
  else if (val == 'q')
    pControl->Stop();
  else if (val == '2')
    pp += 0.01;
  else if (val == '3')
    ii += 0.01;
  else if (val == '4')
    dd += 0.01;
  else if (val == 'w')
    pp -= 0.01;
  else if (val == 'e')
    ii -= 0.01;
  else if (val == 'r')
    dd -= 0.01;

  //  float ypr[3];
  //  GetYPR(&ypr[0]);
  //  int next =  0;//IncPIDCalc(ypr[1]);
  //  //SetPID(pp,ii,dd);
  //  Serial.print("pitch:\t");
  //  Serial.print(ypr[1]);
  //  Serial.print("\tlevel:\t");
  //  Serial.print(next);
  //  Serial.print("\tnext:\t");
  //  Serial.print(LevelPwm);
  //  Serial.print("\tfront:\t");
  //  Serial.print(PinsPwm[SET_FRONT]);
  //  Serial.print("\tafter:\t");
  //  Serial.print(PinsPwm[SET_AFTER]);
  Serial.print("\tpid:\t");
  Serial.print(pp);
  Serial.print("\t");
  Serial.print(ii);
  Serial.print("\t");
  Serial.print(dd);
  Serial.print("\t");
  Serial.print("\n"); 
}
