#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Wire.h>
#include <EEPROM.h>
#include "control.h"

Ccontrol * pControl;

void setup()
{
  Serial.begin(115200); 
  while (Serial.available() && Serial.read());
  //while (!Serial.available());
  //while (Serial.available() && Serial.read());
  if (pControl) delete pControl;
  pControl =  new Ccontrol(10, 3, 11, 9);
  pControl->Init();
}

void loop()
{
  pControl-> Motor();
  int val = Serial.read();
  if (val == 's') pControl->UnlockMotor();
  
  else if (val == '1') pControl->Start();
  else if (val == 'q') pControl->Stop();
  else if (val == '2') pControl->configPID.PitchP += 0.1;
  else if (val == 'w') pControl->configPID.PitchP -= 0.1;
  else if (val == '3') pControl->configPID.PitchI += 0.1;
  else if (val == 'e') pControl->configPID.PitchI -= 0.1;
  else if (val == '4') pControl->configPID.PitchD += 0.1;
  else if (val == 'r') pControl->configPID.PitchD -= 0.1;

  else if (val == '5') pControl->configPID.RollP += 0.1;
  else if (val == 't') pControl->configPID.RollP -= 0.1;
  else if (val == '6') pControl->configPID.RollI += 0.1;
  else if (val == 'y') pControl->configPID.RollI -= 0.1;
  else if (val == '7') pControl->configPID.RollD += 0.1;
  else if (val == 'u') pControl->configPID.RollD -= 0.1;

  else if (val == '8') pControl->configPID.YawP += 0.1;
  else if (val == 'i') pControl->configPID.YawP -= 0.1;
  else if (val == '9') pControl->configPID.YawI += 0.1;
  else if (val == 'o') pControl->configPID.YawI -= 0.1;
  else if (val == '0') pControl->configPID.YawD += 0.1;
  else if (val == 'p') pControl->configPID.YawD -= 0.1;

  else if (val == '=') pControl->AddSpeed();
  else if (val == '-') pControl->SubtractSpeed();

  else if (val == 'c')
    pControl->SaveConfig();

  else if (val == 'b')
    pControl->BalanceAdjust();
}
