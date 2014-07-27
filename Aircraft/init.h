#include "control.h"

void UnlockMotor()
{
  int val = MIN_LEVEL;
  SetAllValue(val);
  Serial.print(val);
  Serial.print(F(".. "));
  delay(1000);
  val = MIN_LEVEL-20;
  SetAllValue(val);
  Serial.print(val);
  Serial.print(F(".. "));
  delay(1000);
  val = MIN_LEVEL+3;
  SetAllValue(val);
  Serial.print(val);
  Serial.print(F(".. "));
  delay(2000);
  SetAllValue(MIN_LEVEL-1);
}

void InitConfiguration()
{ 
  Serial.begin(115200);
  pinMode(PIN_FRONT, OUTPUT);
  pinMode(PIN_AFTER, OUTPUT);
  pinMode(PIN_LEFT, OUTPUT);
  pinMode(PIN_RIGHT, OUTPUT);
  SetAllValue(MIN_LEVEL);
  Serial.println(F("Send any character to activate motors"));
  while (Serial.available() && Serial.read()); // empty buffer
  while (!Serial.available());                 // wait for data
  while (Serial.available() && Serial.read()); // empty buffer again

  Serial.print(F("Testing motor PWM.. \n"));
  UnlockMotor();
}
