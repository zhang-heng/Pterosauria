#include <arduino.h>

typedef struct COORDINATE
{
  int lx;
  int ly;
  int rx;
  int ry;
}Coordinate, *pCoordinate;

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

  Cstick(int leftXPin, int leftYPin, int rightXPin, int rightYPin)
  {
    m_LeftXPin = leftXPin;
    m_LeftYPin = leftYPin;
    m_RightXPin = rightXPin;
    m_RightYPin = rightYPin;
  }
  
  void Init ()
  { 
    m_InitLeftX = analogRead(m_LeftXPin);
    m_InitLeftY = analogRead(m_LeftYPin);
    m_InitRightX = analogRead(m_RightXPin);
    m_InitRightY = analogRead(m_RightYPin);
  }

  void Handle(Coordinate * c)
  {
    c->lx = map(analogRead(m_LeftXPin) - m_InitLeftX, -m_InitLeftX, 1024 - m_InitLeftX, -10, 10);
    c->ly = map(analogRead(m_LeftYPin) - m_InitLeftY, -m_InitLeftY, 1024 - m_InitLeftY, -10, 10);
    c->rx = map(analogRead(m_RightXPin) - m_InitRightX, -m_InitRightX, 1024 - m_InitRightX, -10, 10);
    c->ry = map(analogRead(m_RightYPin) - m_InitRightY, -m_InitRightY, 1024 - m_InitRightY, -10, 10);
  }
};


