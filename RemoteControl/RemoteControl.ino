
#include <ShiftRegLCD.h>

const byte dataPin  = 10;    // SR Data from Arduino pin 10
const byte clockPin = 11;    // SR Clock from Arduino pin 11
const byte enablePin = 12;   // LCD enable from Arduino pin 12
const byte latchPin = 9;

ShiftRegLCD srlcd(dataPin, clockPin, enablePin, 2);

void setup()
{
  uint8_t pitch[8]  = {0x0, 0x4, 0xe, 0x4, 0x4, 0x4, 0x4, 0x0};
  uint8_t roll[8]  = {0x0, 0x0, 0x0, 0x4, 0x1f, 0x0, 0x0, 0x0};
  uint8_t yaw[8]  = {0x0, 0xe, 0x5, 0x4, 0x4, 0x14, 0xe, 0x0};
  uint8_t elevation[8]  = {0x0, 0x1b, 0x0, 0x4, 0x4, 0x0, 0x1b, 0x0};
  srlcd.createChar(0, pitch);
  srlcd.createChar(1, roll);
  srlcd.createChar(2, yaw);
  srlcd.createChar(3, elevation);

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  srlcd.write(i);
}

void loop()
{
  return;
  srlcd.clear();

  for (int r = 0; r < 2; r++)
  {
    for (int c = 0; c < 16; c++)
    {
      srlcd.setCursor(c, r);
      srlcd.print("A");
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 1 << c % 8);
      digitalWrite(latchPin, HIGH);
    }
  }
}

