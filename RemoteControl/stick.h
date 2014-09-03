typedef struct COORDINATE
{
  int lx;
  int ly;
  int rx;
  int ry;
}
  Coordinate, *pCoordinate;

class Cstick
{
 public :
  Cstick(int leftXPin, int leftYPin, int rightXPin, int rightYPin){
    m_LeftXPin = leftXPin;
    m_LeftYPin = leftYPin;
    m_RightXPin = rightXPin;
    m_RightYPin = rightYPin;
    m_InitLeftX = analogRead(m_LeftXPin);
    m_InitLeftY = analogRead(m_LeftYPin);
    m_InitRightY = analogRead(m_RightYPin);
    m_InitRightX = analogRead(m_RightXPin);
  }

  void Read(Coordinate * c){
    c->lx = adjust(analogRead(m_LeftXPin) , m_InitLeftX);
    c->ly = -adjust(analogRead(m_LeftYPin) , m_InitLeftY);
    c->rx = adjust(analogRead(m_RightXPin) , m_InitRightX);
    c->ry = -adjust(analogRead(m_RightYPin) , m_InitRightY);
  }

 private:
  int m_LeftXPin = 0;
  int m_LeftYPin = 0;
  int m_RightXPin = 0;
  int m_RightYPin = 0;

  int m_InitLeftX = 0;
  int m_InitLeftY = 0;
  int m_InitRightX = 0;
  int m_InitRightY = 0;

  int adjust(int v, int initV){
    int val = v - initV;
    if (abs(val)<20) return 0;
    return map(val, -initV, 1024- initV, -10, 10);
    return val;
  }
};
