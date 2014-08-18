
class Clcd
{
  public :
    ShiftRegLCD *srlcd;
    int m_LatchPin;
    int m_DataPin;
    int m_ClockPin;

    Clcd (int latchPin, int dataPin, int clockPin)
    {
      m_LatchPin = latchPin;
      m_DataPin = dataPin;
      m_ClockPin = clockPin;
      pinMode(m_LatchPin, OUTPUT);
      pinMode(m_DataPin, OUTPUT);
      pinMode(m_ClockPin, OUTPUT);

      srlcd = new ShiftRegLCD(m_DataPin, m_ClockPin, m_LatchPin, 2);
      uint8_t pitch[8] = {0x0, 0x4, 0xe, 0x4, 0x4, 0x4, 0x4, 0x0};
      uint8_t roll[8] = {0x0, 0x0, 0x0, 0x4, 0x1f, 0x0, 0x0, 0x0};
      uint8_t yaw[8] = {0x0, 0xe, 0x5, 0x4, 0x4, 0x14, 0xe, 0x0};
      uint8_t elevation[8] = {0x0, 0x1b, 0x0, 0x4, 0x4, 0x0, 0x1b, 0x0};
      srlcd->createChar(0, pitch);
      srlcd->createChar(1, roll);
      srlcd->createChar(2, yaw);
      srlcd->createChar(3, elevation);
      srlcd->write(0);
      srlcd->write(1);
      srlcd->write(2);
      srlcd->write(3);
    }
    void Handle()
    {
    }
};
