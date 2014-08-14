
#include <ShiftRegLCD.h>

const byte clockPin = 2;
const byte dataOutPin = 3;
const byte latchLcdPin = 4;
const byte latchOutPin = 5;
const byte latchInPin = 6;
const byte dataInPin  = 7;

ShiftRegLCD srlcd(dataOutPin, clockPin, latchLcdPin, 2);

void setup()
{
  Serial.begin(9600);
  uint8_t pitch[8]  = {0x0, 0x4, 0xe, 0x4, 0x4, 0x4, 0x4, 0x0};
  uint8_t roll[8]  = {0x0, 0x0, 0x0, 0x4, 0x1f, 0x0, 0x0, 0x0};
  uint8_t yaw[8]  = {0x0, 0xe, 0x5, 0x4, 0x4, 0x14, 0xe, 0x0};
  uint8_t elevation[8]  = {0x0, 0x1b, 0x0, 0x4, 0x4, 0x0, 0x1b, 0x0};
  srlcd.createChar(0, pitch);
  srlcd.createChar(1, roll);
  srlcd.createChar(2, yaw);
  srlcd.createChar(3, elevation);

  pinMode(clockPin, OUTPUT);
  pinMode(latchLcdPin, OUTPUT);
  pinMode(latchOutPin, OUTPUT);
  pinMode(latchInPin, OUTPUT);
  pinMode(dataOutPin, OUTPUT);
  pinMode(dataInPin, INPUT);
  srlcd.write(0);
  srlcd.write(1);
  srlcd.write(2);
  srlcd.write(3);
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
      digitalWrite(latchOutPin, LOW);
      shiftOut(dataOutPin, clockPin, MSBFIRST, 1 << c % 8);
      digitalWrite(latchOutPin, HIGH);
    }
  }

  digitalWrite(latchInPin, LOW);
  byte c = shiftIn(dataInPin, clockPin, MSBFIRST);
  digitalWrite(latchInPin, HIGH);
  c = (shiftIn(dataInPin, clockPin, MSBFIRST) << 7) | c >> 1 ;
  Serial.print(c & 0x80 ? 1 : 0);
  Serial.print(c & 0x40 ? 1 : 0);
  Serial.print(c & 0x20 ? 1 : 0);
  Serial.print(c & 0x10 ? 1 : 0);
  Serial.print(c & 0x08 ? 1 : 0);
  Serial.print(c & 0x04 ? 1 : 0);
  Serial.print(c & 0x02 ? 1 : 0);
  Serial.print(c & 0x01 ? 1 : 0);
  Serial.print("\n");
  delay(100);
}
