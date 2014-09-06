class Cleds
{
public:
  int m_LatchPin;
  int m_DataPin;
  int m_ClockPin;

  short HeaderMask;
  short LeftMask[5];
  short RightMask[5];

  short m_Value;

  long m_LastTime ;
  bool m_Light;

  void Write(short v){
    char* c = (char*)&v;
    digitalWrite(m_LatchPin, LOW);
    shiftOut(m_DataPin, m_ClockPin, MSBFIRST, c[0]);
    shiftOut(m_DataPin, m_ClockPin, MSBFIRST, c[1]);
    digitalWrite(m_LatchPin, HIGH);
  }

  Cleds(int latchPin, int dataPin, int clockPin){
    m_Light = false;
    HeaderMask = 0x0004;
    
    LeftMask[0] =0x8000;
    LeftMask[1] =0x4000;
    LeftMask[2] =0x2000;
    LeftMask[3] =0x1000;
    LeftMask[4] =0x0800; 
    
    RightMask[0] =0x0080;
    RightMask[1] =0x0040;
    RightMask[2] =0x0020;
    RightMask[3] =0x0010;
    RightMask[4] =0x0008;      
    m_LatchPin = latchPin;
    m_DataPin = dataPin;
    m_ClockPin = clockPin;
    pinMode(m_LatchPin, OUTPUT);
    pinMode(m_DataPin, OUTPUT);
    pinMode(m_ClockPin, OUTPUT);
  }

  void NetBlink(){
    m_Value^= HeaderMask;
    Handle();
  }

  void NetOff(){
    m_Value &=~HeaderMask;
    Handle();
  }

  void Handle(){
    Write(m_Value);
  }

  void NetOn(){
    m_Value |=HeaderMask ;
    Handle();
  }
};



