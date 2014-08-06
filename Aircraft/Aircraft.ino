#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

#include "init.h" 
#include "pid.h"

double pp = 0;
double ii = 0;
double dd = 0;

void setup()
{
  InitConfiguration();
  IncPIDInit(pp, ii, dd, 0);
  LevelPwm = 130;
}

void loop()
{
  float ypr[3];
  GetYPR(&ypr[0]); 
  int next =  0;//IncPIDCalc(ypr[1]);
  SetPID(pp,ii,dd);
  Serial.print("pitch:\t"); 
  Serial.print(ypr[1]);
  Serial.print("\t"); 
  Serial.print("level:\t"); 
  Serial.print(next);
  Serial.print("\t"); 
  Serial.print("next:\t"); 
  Serial.print(LevelPwm);
  Serial.print("\t"); 
  Serial.print("front:\t"); 
  Serial.print(PinsPwm[SET_FRONT]);
  Serial.print("\t"); 
  Serial.print("after:\t");
  Serial.print(PinsPwm[SET_AFTER]);  
  Serial.print("\t"); 
  Serial.print("pid:\t");
  Serial.print(pp);  
  Serial.print("\t"); 
  Serial.print(ii);  
  Serial.print("\t"); 
  Serial.print(dd);  
  Serial.print("\t"); 
  Serial.print("\n");

  SetPitch(next);
  int val = Serial.read();
  if (val =='1' )
    ++LevelPwm;
  else if (val == 'q')
    --LevelPwm;  
  else if (val == '2')
    pp+=0.01;  
  else if (val == '3')
    ii+=0.01;  
  else if (val == '4')
    dd+=0.01;  
  else if (val == 'w')
    pp-=0.01;  
  else if (val == 'e')
    ii-=0.01;  
  else if (val == 'r')
    dd-=0.01;  
  else if (val == 'p')
  {
    LevelPwm = 130; 
  }
}








