#include <ShiftRegLCD.h>
//#include <SPI.h>
//#include <Mirf.h>
//#include <nRF24L01.h>
//#include <MirfHardwareSpiDriver.h>

#include "lcd.h"
//#include "stick.h"
//#include "leds.h"
//#include "buttn.h"
//#include "connect.h"

//Cleds *leds;
Clcd *lcd;
//Cbuttn *buttn;
//Cstick *stick;
//Cconnect *conn;

void setup()
{
  Serial.begin(9600);
  //leds = new Cleds(5, 3, 2);
  lcd = new Clcd(4, 3, 2);
  //buttn = new Cbuttn (6, 7, 2);
  //stick = new Cstick(2, 0, 1, 3);
  //conn = new Cconnect (10, 9);
  //stick->Init();
}

void loop()
{
  //Coordinate c;
  analogRead(0);
  analogRead(1);
  analogRead(2);
  analogRead(3);
  Serial.print("\n");
  delay(500);
  //lcd->srlcd->clear();
  //stick->Handle(&c);
//  lcd->srlcd->setCursor(0, 0);
//  lcd->srlcd->print(c.lx);
//  lcd->srlcd->print(" ");
//  lcd->srlcd->print(c.ly);
//  lcd->srlcd->print(" ");
//  lcd->srlcd->print(c.rx);
//  lcd->srlcd->print(" ");
//  lcd->srlcd->print(c.ry);
//  lcd->srlcd->print(" ");
//  for (int i = 0; i < 10; i++)
//  {
//    lcd->srlcd->print(i);
//    delay(1000);
//  }
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

