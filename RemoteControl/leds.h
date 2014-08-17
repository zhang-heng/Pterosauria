#include <arduino.h>
class Cleds
{
  public:
    int m_LatchPin;
    int m_DataPin;
    int m_ClockPin;

    short HeaderBlue = 0x0100;

    void Write(byte* b)
    {
      digitalWrite(m_LatchPin, LOW);
      shiftOut(m_DataPin, m_ClockPin, MSBFIRST, b[0]);
      shiftOut(m_DataPin, m_ClockPin, MSBFIRST, b[1]);
      digitalWrite(m_LatchPin, HIGH);
    }

    Cleds(int latchPin, int dataPin, int clockPin)
    {
      m_LatchPin = latchPin;
      m_DataPin = dataPin;
      m_ClockPin = clockPin;
      pinMode(m_LatchPin, OUTPUT);
      pinMode(m_DataPin, OUTPUT);
      pinMode(m_ClockPin, OUTPUT);
    }

    void Handle()
    {
    }
};
