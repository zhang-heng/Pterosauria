 
typedef struct buttons
{
  bool a1;
  bool a2;
  bool b1;
  bool b2;
  bool b3;
};

class Cbuttn
{
  public :
    int m_LatchPin;
    int m_DataPin;
    int m_ClockPin;

    void Read(buttons * b)
    {
      digitalWrite(m_LatchPin, LOW);
      byte c = shiftIn(m_DataPin, m_ClockPin, MSBFIRST);
      digitalWrite(m_LatchPin, HIGH);

      b->a1 = c & 0x04;
      b->a2 = c & 0x02;
      b->b1 = !(c & 0x20);
      b->b2 = !(c & 0x08);
      b->b3 = !(c & 0x10);
    }

    Cbuttn (int latchPin, int dataPin, int clockPin)
    {
      m_LatchPin = latchPin;
      m_DataPin = dataPin;
      m_ClockPin = clockPin;
      pinMode(m_LatchPin, OUTPUT);
      pinMode(m_DataPin, INPUT);
      pinMode(m_ClockPin, OUTPUT);
    }

    void Handle(buttons * bt)
    {
      Read(bt);
    }
};

