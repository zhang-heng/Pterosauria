#include <ShiftRegLCD.h>
#include <SPI.h>
#include <NetDefine.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <arduino.h>

#include "stick.h"
#include "connect.h"
#include "lcd.h"
#include "leds.h"
#include "buttn.h"

Cstick *stick;
Cleds *leds;
Clcd *lcd;
Cbuttn *buttn;
Cconnect *conn;

void setup()
{
  Serial.begin(115200);
  conn = new Cconnect (10, 9);
  leds = new Cleds(5, 3, 2);
  lcd = new Clcd(4, 3, 2);
  buttn = new Cbuttn (6, 7, 2);
  stick = new Cstick(0, 1, 3, 2);
  Serial.println ("init finish");
}

float pitch = 0;
float pitchP = 0;
float pitchI = 0;
float pitchD = 0;

float roll = 0;
float rollP = 0;
float rollI = 0;
float rollD = 0;

float yaw = 0;
float yawP = 0;
float yawI = 0;
float yawD = 0;

float power = 0;

float v = 0;
Buttons bs = {
  0, 0, 0, 0, 0
};

void loop()
{
  buttn->Read(&bs);
  conn->GetValueByType(TYPE_POWER, v);
  conn->GetValueByType(TYPE_PITCH, pitch);
  conn->GetValueByType(TYPE_PITCH_P, pitchP);
  conn->GetValueByType(TYPE_PITCH_I, pitchI);
  conn->GetValueByType(TYPE_PITCH_D, pitchD);

  conn->GetValueByType(TYPE_ROLL, roll);
  conn->GetValueByType(TYPE_ROLL_P, rollP);
  conn->GetValueByType(TYPE_ROLL_I, rollI);
  conn->GetValueByType(TYPE_ROLL_D, rollD);

  conn->GetValueByType(TYPE_YAW, yaw);
  conn->GetValueByType(TYPE_YAW_P, yawP);
  conn->GetValueByType(TYPE_YAW_I, yawI);
  conn->GetValueByType(TYPE_YAW_D, yawD);

  //conn->GetValueByType(TYPE_POWER, power);

  if (bs.a1)
  {
    if (bs.a2)
    {
      conn->GetValueByType(TYPE_UNLOCK, v);
      leds->NetBlink();
    } else
    {
      leds->NetOn();
    }
    conn->GetValueByType(TYPE_SELF_STATIONARY, v);
  }
  else
  {
    leds->NetOff();
    if (bs.b1)
    {
      conn->GetValueByType(TYPE_INIT_BALANCE, v);
    }
    conn->GetValueByType(TYPE_TIME_SYN, v);
  }

  lcd->Show(pitch, roll, yaw, 0);
  lcd->ShowPower(power);
  lcd->SetSignal(conn->GetSuccessCount());
}
