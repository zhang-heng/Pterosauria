class Csonar
{
 private:
  NewPing * m_sonar;
 public:
  Csonar(){
    int trigPin = A6;
    int echoPin = A7;
    m_sonar = new NewPing(trigPin, echoPin);
  }
  int GetDistance(){
    unsigned int uS = m_sonar->ping();
    return uS / US_ROUNDTRIP_CM;
  }
};
