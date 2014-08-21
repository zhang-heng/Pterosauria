#include <ShiftRegLCD.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#include "lcd.h"
#include "stick.h"
#include "leds.h"
#include "buttn.h"
#include "connect.h"

Cleds *leds;
Clcd *lcd;
Cbuttn *buttn;
Cstick *stick;
Cconnect *conn;

void setup()
{
  Serial.begin(115200);
  leds = new Cleds(5, 3, 2);
  lcd = new Clcd(4, 3, 2);
  buttn = new Cbuttn (6, 7, 2);
  stick = new Cstick(0, 1, 3, 2);
  conn = new Cconnect (10, 9); 
  Serial.println ("init finish");
}

void loop()
{
  conn->Handle();
}



