
#include <ShiftRegLCD.h>

const byte dataPin  = 10;    // SR Data from Arduino pin 10
const byte clockPin = 11;    // SR Clock from Arduino pin 11
const byte enablePin = 12;   // LCD enable from Arduino pin 12

ShiftRegLCD srlcd(dataPin, clockPin, enablePin, 2);

void setup()
{
}

void loop()
{
  srlcd.clear();
  for (int r = 0; r < 2; r++)
  {
    for (int c = 0; c < 16; c++)
    {
      srlcd.setCursor(c, r);
      srlcd.print("A");
      delay(500);
    }
  }
}

