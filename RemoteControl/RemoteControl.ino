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
  stick = new Cstick(3,2,0,1);
  conn = new Cconnect (10, 9);
  leds = new Cleds(5, 3, 2);
  lcd = new Clcd(4, 3, 2);
  buttn = new Cbuttn (6, 7, 2);
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

  conn->GetValueByType(TYPE_PITCH_CURRENT, pitch);
  conn->GetValueByType(TYPE_ROLL_CURRENT, roll);
  conn->GetValueByType(TYPE_YAW_CURRENT, yaw);
  conn->GetValueByType(TYPE_ELEVATION_CURRENT, ele);

  if (bs.b3){
    conn->CommandByType(TYPE_WRITE_CONFIG);
  }
  if (bs.a1){
    if (bs.a2){//飞行端解锁,飞行,自稳.
      leds->NetBlink();
      conn->CommandByType(TYPE_FLY);
      if (!cc.lx && !cc.ly && !cc.rx && !cc.ry){
        conn->CommandByType(TYPE_SELF_STATIONARY);
      }else{
        Serial.print("stick:");
        Serial.print("\t");
        Serial.print(cc.lx);
        Serial.print("\t");
        Serial.print(cc.ly);
        Serial.print("\t");
        Serial.print(cc.rx);
        Serial.print("\t");
        Serial.print(cc.ry);
        Serial.print("\n");
      }
      if(cc.lx) {
        conn->SetValueByType(TYPE_ROLL_DESIRED, cc.lx);
        leds->RollBlink();
      }else leds->RollOff();
      if(cc.ly){
        conn->SetValueByType(TYPE_PITCH_DESIRED, cc.ly);
        leds->PitchBlink();
      }else leds->PitchOff();
      if(cc.rx) {
        conn->SetValueByType(TYPE_YAW_DESIRED, cc.rx);
        leds->YawBlink();
      }else leds->YawOff();
      if(cc.ry) {
        conn->SetValueByType(TYPE_ELEVATION_DESIRED, cc.ry);
        leds->EleBlink();
      }else leds->EleOff();
    }
    else{
      leds->NetOn();
      conn->CommandByType(TYPE_LAND);
    }
  }
  else{
    leds->NetOff();
    conn->CommandByType(TYPE_LAND);

    if (bs.b1){
      conn->CommandByType(TYPE_INIT_BALANCE);
    }
    conn->CommandByType(TYPE_TIME_SYN);
  }

  lcd->Show(pitch, roll, yaw, ele);
  lcd->ShowPower(power);
  lcd->SetSignal(conn->GetSuccessCount());
  for(int i =0;i<10;i++)
    SerialControl();
}

void SerialControl(){
  switch(Serial.read()){
  case '1':conn->GetValueByType(TYPE_PITCH_P, 0.01,pitchP);break;
  case 'q':conn->GetValueByType(TYPE_PITCH_P, -0.01,pitchP);break;
  case '2':conn->GetValueByType(TYPE_PITCH_I, 0.01,pitchI);break;
  case 'w':conn->GetValueByType(TYPE_PITCH_I, -0.01,pitchI);break;
  case '3':conn->GetValueByType(TYPE_PITCH_D, 0.01,pitchD);break;
  case 'e':conn->GetValueByType(TYPE_PITCH_D, -0.01,pitchD);break;
  case '!':conn->GetValueByType(TYPE_PITCH_P, 0.1,pitchP);break;
  case 'Q':conn->GetValueByType(TYPE_PITCH_P, -0.1,pitchP);break;
  case '@':conn->GetValueByType(TYPE_PITCH_I, 0.1,pitchI);break;
  case 'W':conn->GetValueByType(TYPE_PITCH_I, -0.1,pitchI);break;
  case '#':conn->GetValueByType(TYPE_PITCH_D, 0.1,pitchD);break;
  case 'E':conn->GetValueByType(TYPE_PITCH_D, -0.1,pitchD);break;

  case '4':conn->GetValueByType(TYPE_ROLL_P, 0.01,rollP);break;
  case 'r':conn->GetValueByType(TYPE_ROLL_P, -0.01,rollP);break;
  case '5':conn->GetValueByType(TYPE_ROLL_I, 0.01,rollI);break;
  case 't':conn->GetValueByType(TYPE_ROLL_I, -0.01,rollI);break;
  case '6':conn->GetValueByType(TYPE_ROLL_D, 0.01,rollD);break;
  case 'y':conn->GetValueByType(TYPE_ROLL_D, -0.01,rollD);break;
  case '7':conn->GetValueByType(TYPE_YAW_P, 0.01,yawP);break;
  case 'u':conn->GetValueByType(TYPE_YAW_P, -0.01,yawP);break;
  case '8':conn->GetValueByType(TYPE_YAW_I, 0.01,yawI);break;
  case 'i':conn->GetValueByType(TYPE_YAW_I, -0.01,yawI);break;
  case '9':conn->GetValueByType(TYPE_YAW_D, 0.01,yawD);break;
  case 'o':conn->GetValueByType(TYPE_YAW_D, -0.01,yawD);break;
  case '0':conn->GetValueByType(TYPE_ELEVATION_P, 0.01,eleP);break;
  case 'p':conn->GetValueByType(TYPE_ELEVATION_P, -0.01,eleP);break;
  case '-':conn->GetValueByType(TYPE_ELEVATION_I, 0.01,eleI);break;
  case '[':conn->GetValueByType(TYPE_ELEVATION_I, -0.01,eleI);break;
  case '=':conn->GetValueByType(TYPE_ELEVATION_D, 0.01,eleD);break;
  case ']':conn->GetValueByType(TYPE_ELEVATION_D, -0.01,eleD);break;
  case -1: return;
  }

  conn->GetValueByType(TYPE_PITCH_P, 0,pitchP);
  conn->GetValueByType(TYPE_PITCH_I, 0,pitchI);
  conn->GetValueByType(TYPE_PITCH_D, 0,pitchD);
  conn->GetValueByType(TYPE_ROLL_P, 0,rollP);
  conn->GetValueByType(TYPE_ROLL_I, 0,rollI);
  conn->GetValueByType(TYPE_ROLL_D, 0,rollD);
  conn->GetValueByType(TYPE_YAW_P, 0,yawP);
  conn->GetValueByType(TYPE_YAW_I, 0,yawI);
  conn->GetValueByType(TYPE_YAW_D, 0,yawD);
  conn->GetValueByType(TYPE_ELEVATION_P, 0,eleP);
  conn->GetValueByType(TYPE_ELEVATION_I, 0,eleI);
  conn->GetValueByType(TYPE_ELEVATION_D, 0,eleD);
  Serial.print("pitchPID:");
  Serial.print(pitchP);Serial.print("\t");
  Serial.print(pitchI);Serial.print("\t");
  Serial.print(pitchD);Serial.print("\t");
  Serial.print("rollPID:");
  Serial.print(rollP);Serial.print("\t");
  Serial.print(rollI);Serial.print("\t");
  Serial.print(rollD);Serial.print("\t");
  Serial.print("yawPID:");
  Serial.print(yawP);Serial.print("\t");
  Serial.print(yawI);Serial.print("\t");
  Serial.print(yawD);Serial.print("\t");
  Serial.print("elePID:");
  Serial.print(eleP);Serial.print("\t");
  Serial.print(eleI);Serial.print("\t");
  Serial.print(eleD);Serial.print("\n");
}
