#include <arduino.h>

typedef struct COORDINATE
{
  int lx;
  int ly;
  int rx;
  int ry;
} Coordinate, *pCoordinate;

class Cstick
{
  public :
    int m_LeftXPin = 0;
    int m_LeftYPin = 0;
    int m_RightXPin = 0;
    int m_RightYPin = 0;

    int m_InitLeftX = 0;
    int m_InitLeftY = 0;
    int m_InitRightX = 0;
    int m_InitRightY = 0;
    bool m_Inited = false;

    Cstick(int leftXPin, int leftYPin, int rightXPin, int rightYPin)
    {
      m_LeftXPin = leftXPin;
      m_LeftYPin = leftYPin;
      m_RightXPin = rightXPin;
      m_RightYPin = rightYPin;
    }
    void Init ()
    {
      if (m_Inited) return;
      m_InitLeftX = analogRead(m_LeftXPin);
      //m_InitLeftY = analogRead(m_LeftYPin);
      //m_InitRightX = analogRead(m_RightXPin);
      //m_InitRightY = analogRead(m_RightYPin);
    }

    void Handle(Coordinate * c)
    {
      c->lx = map(1024 - analogRead(m_LeftXPin) - m_InitLeftX, -512 + m_InitLeftX / 2, 512 - m_InitLeftX / 2, -10, 10);
      c->ly = map(1024 - analogRead(m_LeftYPin) - m_InitLeftY, -512 + m_InitLeftY / 2, 512 - m_InitLeftY / 2, -10, 10);
      c->rx = map(1024 - analogRead(m_RightXPin) - m_InitRightX, -512 + m_InitRightX / 2, 512 - m_InitRightX / 2, -10, 10);
      c->ry = map(1024 - analogRead(m_RightYPin) - m_InitRightY, -512 + m_InitRightY / 2, 512 - m_InitRightY / 2, -10, 10);
    }
};
