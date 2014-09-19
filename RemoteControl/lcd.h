#include <ShiftRegLCD.h>

enum TypePic
  {
    PIC_PITCH = 0,
    PIC_ROLL,
    PIC_YAW,
    PIC_ELEVATION,
    PIC_POWER,
    PIC_SIGNAL,
  };

class Clcd
{
 public :
  ShiftRegLCD *srlcd;
  int m_LatchPin;
  int m_DataPin;
  int m_ClockPin;

  Clcd (int latchPin, int dataPin, int clockPin){
    m_LatchPin = latchPin;
    m_DataPin = dataPin;
    m_ClockPin = clockPin;
    pinMode(m_LatchPin, OUTPUT);
    pinMode(m_DataPin, OUTPUT);
    pinMode(m_ClockPin, OUTPUT);

    srlcd = new ShiftRegLCD(m_DataPin, m_ClockPin, m_LatchPin, 2);

  }

  void Show(float p, float r, float y, float e){
    ShowP((int)p);
    ShowI((int)r);
    ShowY((int)y);
    ShowE((int)e);
  }

  //0 - 100
  void ShowPower(int v){
    int p = map (v, 0, 100, 0, 7);
    uint8_t power[8] = {0x0, 0x08, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x0};
    for (int j = 0; j <= p; j++) power[7 - j] |= 0x1;
    srlcd->createChar(PIC_POWER,  power);
    srlcd->setCursor(0, 0);
    srlcd->write(PIC_POWER);
  }

  void SetSignal(int v){
    int p = map (v, 0, 100, 0, 7);
    uint8_t signal[8] = {0x0, 0x14, 0x14, 0x18, 0x10, 0x10, 0x10, 0x0};
    for (int j = 0 ; j <= p; j++) signal[7 - j] |= 0x1;
    srlcd->createChar(PIC_SIGNAL,  signal);
    srlcd->setCursor(1, 0);
    srlcd->write(PIC_SIGNAL);
  }

 private:
  void ShowP(int v){
    uint8_t pic[8] = {0x0, 0x4, 0xe, 0x4, 0x4, 0x4, 0x4, 0x0};
    v<0?pic[7]|=0x01:pic[0]|=0x01;
    srlcd->createChar(PIC_PITCH, pic);
    srlcd->setCursor(0, 1);
    srlcd->write(PIC_PITCH);
    srlcd->print(abs(v));
    Interval(v);
  }

  void ShowI(int v){
    uint8_t pic[8] = {0x0, 0x0, 0x0, 0x4, 0x1f, 0x0, 0x0, 0x0};
    v<0?pic[7]|=0x01:pic[0]|=0x01;
    srlcd->createChar(PIC_ROLL, pic);
    srlcd->setCursor(4, 1);
    srlcd->write(PIC_ROLL);
    srlcd->print(abs(v));
    Interval(v);
  }

  void ShowY(int v){
    uint8_t pic[8] = {0x0, 0xe, 0x5, 0x4, 0x4, 0x14, 0xe, 0x0};
    v<0?pic[7]|=0x01:pic[0]|=0x01;
    srlcd->createChar(PIC_YAW, pic);
    srlcd->setCursor(8, 1);
    srlcd->write(PIC_YAW);
    srlcd->print(abs(v));
    Interval(v);
  }

  void ShowE(int v){
    uint8_t pic[8] = {0x0, 0x1b, 0x0, 0x4, 0x4, 0x0, 0x1b, 0x0};
    v<0?pic[7]|=0x01:pic[0]|=0x01;
    srlcd->createChar(PIC_ELEVATION, pic);
    srlcd->setCursor(12, 1);
    srlcd->write(PIC_ELEVATION);
    srlcd->print(abs(v));
    Interval(v);
  }
  
  void Interval(int v){
    if (v/100) srlcd->print("");
    else if (v/10) srlcd->print(" ");
    else srlcd->print("  ");
  }
};
