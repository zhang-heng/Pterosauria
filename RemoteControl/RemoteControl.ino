#include <ShiftRegLCD.h>
#include <SPI.h>
#include <NetDefine.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <Arduino.h>

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

void setup(){
  Serial.begin(115200);
  conn = new Cconnect (10, 9);
  leds = new Cleds(5, 3, 2);
  lcd = new Clcd(4, 3, 2);
  buttn = new Cbuttn (6, 7, 2);
  stick = new Cstick(2, 3, 1, 0);
  Serial.println ("init finish");
}

float pitch = 0;
float roll = 0;
float yaw = 0;
float ele = 0;
float pitchP = 0;
float pitchI = 0;
float pitchD = 0;
float rollP = 0;
float rollI = 0;
float rollD = 0;
float yawP = 0;
float yawI = 0;
float yawD = 0;
float eleP = 0;
float eleI = 0;
float eleD = 0;
float power = 0;

ulong motorA =0;
ulong motorB =0;
ulong motorC =0;
ulong motorD =0;

void loop(){
  Buttons bs = {0, 0, 0, 0, 0};
  buttn->Read(&bs);

  Coordinate cc;
  stick->Read(&cc);

  conn->GetValueByType(TYPE_POWER, power);

  conn->GetValueByType(TYPE_PITCH, pitch);
  conn->GetValueByType(TYPE_ROLL, roll);
  conn->GetValueByType(TYPE_YAW, yaw);
  conn->GetValueByType(TYPE_ELEVATION, ele);

  ulong  stickv [4] = {
    cc.lx, cc.ly, cc.rx, cc.ry
  };

  if (bs.a1){
    if (bs.a2){//飞行端解锁,飞行,自稳.
      leds->NetBlink();
      if(cc.lx) conn->SetValueByType(TYPE_PITCH, cc.lx);
      if(cc.ly) conn->SetValueByType(TYPE_ROLL, cc.ly);
      if(cc.rx) conn->SetValueByType(TYPE_ELEVATION, cc.rx);
      if(cc.ry) conn->SetValueByType(TYPE_YAW, cc.ry);
      if (!cc.lx && !cc.ly && !cc.rx && !cc.ry)
        conn->CommandByType(TYPE_SELF_STATIONARY);
    }
    else{
      leds->NetOn();
      conn->CommandByType(TYPE_LAND);
    }
  }
  else{
    leds->NetOff();
    conn->GetValueByType(TYPE_PITCH_P, pitchP);
    conn->GetValueByType(TYPE_PITCH_I, pitchI);
    conn->GetValueByType(TYPE_PITCH_D, pitchD);
    conn->GetValueByType(TYPE_ROLL_P, rollP);
    conn->GetValueByType(TYPE_ROLL_I, rollI);
    conn->GetValueByType(TYPE_ROLL_D, rollD);
    conn->GetValueByType(TYPE_YAW_P, yawP);
    conn->GetValueByType(TYPE_YAW_I, yawI);
    conn->GetValueByType(TYPE_YAW_D, yawD);
    conn->GetValueByType(TYPE_ELEVATION_P, eleP);
    conn->GetValueByType(TYPE_ELEVATION_I, eleI);
    conn->GetValueByType(TYPE_ELEVATION_D, eleD);

    if (bs.b1){
      conn->CommandByType(TYPE_INIT_BALANCE);
    }
    conn->CommandByType(TYPE_TIME_SYN);
  }

  lcd->Show(pitch, roll, yaw, 0);
  lcd->ShowPower(power);
  lcd->SetSignal(conn->GetSuccessCount());
}

void SerialControl(){
  switch( Serial.read()){
  case 'a':
    conn->GetValueByType(TYPE_MOTOR_A, 1,motorA);
    break;
  case 'z':
    conn->GetValueByType(TYPE_MOTOR_A, -1,motorA);
    break;
  case 's':
    conn->GetValueByType(TYPE_MOTOR_B, 1,motorB);
    break;
  case 'x':
    conn->GetValueByType(TYPE_MOTOR_B, -1,motorB);
    break;
  case 'd':
    conn->GetValueByType(TYPE_MOTOR_C, 1,motorC);
    break;
  case 'c':
    conn->GetValueByType(TYPE_MOTOR_C, -1,motorC);
    break;
  case 'f':
    conn->GetValueByType(TYPE_MOTOR_D, 1,motorD);
    break;
  case 'v':
    conn->GetValueByType(TYPE_MOTOR_D, -1,motorD);
    break;

  case '1':
    conn->GetValueByType(TYPE_PITCH_P, 0.01,pitchP);
    break;
  case 'q':
    conn->GetValueByType(TYPE_PITCH_P, -0.01,pitchP);
    break;
  case '2':
    conn->GetValueByType(TYPE_PITCH_I, 0.01,pitchI);
    break;
  case 'w':
    conn->GetValueByType(TYPE_PITCH_I, -0.01,pitchI);
    break;
  case '3':
    conn->GetValueByType(TYPE_PITCH_D, 0.01,pitchD);
    break;
  case 'e':
    conn->GetValueByType(TYPE_PITCH_D, -0.01,pitchD);
    break;

  case '4':
    conn->GetValueByType(TYPE_ROLL_P, 0.01,rollP);
    break;
  case 'r':
    conn->GetValueByType(TYPE_ROLL_P, -0.01,rollP);
    break;
  case '5':
    conn->GetValueByType(TYPE_ROLL_I, 0.01,rollI);
    break;
  case 't':
    conn->GetValueByType(TYPE_ROLL_I, -0.01,rollI);
    break;
  case '6':
    conn->GetValueByType(TYPE_ROLL_D, 0.01,rollD);
    break;
  case 'y':
    conn->GetValueByType(TYPE_ROLL_D, -0.01,rollD);
    break;

  case '7':
    conn->GetValueByType(TYPE_YAW_P, 0.01,yawP);
    break;
  case 'u':
    conn->GetValueByType(TYPE_YAW_P, -0.01,yawP);
    break;
  case '8':
    conn->GetValueByType(TYPE_YAW_I, 0.01,yawI);
    break;
  case 'i':
    conn->GetValueByType(TYPE_YAW_I, -0.01,yawI);
    break;
  case '9':
    conn->GetValueByType(TYPE_YAW_D, 0.01,yawD);
    break;
  case 'o':
    conn->GetValueByType(TYPE_YAW_D, -0.01,yawD);
    break;

  case '0':
    conn->GetValueByType(TYPE_ELEVATION_P, 0.01,eleP);
    break;
  case 'p':
    conn->GetValueByType(TYPE_ELEVATION_P, -0.01,eleP);
    break;
  case '-':
    conn->GetValueByType(TYPE_ELEVATION_I, 0.01,eleI);
    break;
  case '[':
    conn->GetValueByType(TYPE_ELEVATION_I, -0.01,eleI);
    break;
  case '=':
    conn->GetValueByType(TYPE_ELEVATION_D, 0.01,eleD);
    break;
  case ']':
    conn->GetValueByType(TYPE_ELEVATION_D, -0.01,eleD);
    break;
  }
}
