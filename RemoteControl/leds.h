
class Cleds
{
public:
  int m_LatchPin;
  int m_DataPin;
  int m_ClockPin;

  short HeaderMask = 0x0004;
  short LeftMask[5] = {0x8000, 0x4000, 0x2000, 0x1000, 0x0800};
  short RightMask[5] = {0x0080, 0x0040, 0x0020, 0x0010, 0x0008};

  short m_Value = 0x0000;

  long m_LastTime = 0;
  bool m_Light = false;

  void Write(short v)
  {
    char* c = (char*)&v;
    digitalWrite(m_LatchPin, LOW);
    shiftOut(m_DataPin, m_ClockPin, MSBFIRST, c[0]);
    shiftOut(m_DataPin, m_ClockPin, MSBFIRST, c[1]);
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
    long t = millis();
    if(m_Light && t - m_LastTime > 200)
    {
      m_LastTime = t;
      Write(0);
    }
    if(!m_Light && t - m_LastTime > 500)
    {
      m_LastTime = t;
      Write(m_Value);
    }
  }
};



