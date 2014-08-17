
#include <ShiftRegLCD.h>

#include "buttn.h"
#include "lcd.h"
#include "leds.h"
#include "stick.h"

Cleds *leds;
Clcd *lcd;
Cbuttn *buttn;
Cstick *stick;

Coordinate coordinate;

void setup()
{
  Serial.begin(9600);
  leds = new Cleds(5, 3, 2);
  lcd = new Clcd(4, 3, 2);
  buttn = new Cbuttn (6, 7, 2);
  stick = new Cstick(2, 0, 1, 3);
}

void loop()
{
  stick->Init();
  lcd->srlcd->setCursor(0, 0);
  lcd->srlcd->print(stick->m_LeftXPin);
  lcd->srlcd->print(" ");
  lcd->srlcd->print(stick->m_LeftYPin);
  lcd->srlcd->print(" ");
  lcd->srlcd->print(stick->m_RightXPin);
  lcd->srlcd->print(" ");
  lcd->srlcd->print(stick->m_RightYPin);
  lcd->srlcd->print(" ");
  for (int i = 0; i < 10; i++)
  {
    lcd->srlcd->print(i);
    delay(1000);
  }
//
//  stick->Handle(&coordinate);
//  lcd->srlcd->setCursor(0, 0);
//  lcd->srlcd->print(coordinate.lx);
//  lcd->srlcd->print(" ");
//  lcd->srlcd->print(coordinate.ly);
//  lcd->srlcd->print("        ");
//  lcd->srlcd->setCursor(0, 1);
//  lcd->srlcd->print(coordinate.rx);
//  lcd->srlcd->print(" ");
//  lcd->srlcd->print(coordinate.ry);
//  lcd->srlcd->print("        ");
  // delay(100);
}
