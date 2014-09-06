class Csonar
{
 private:
  NewPing * m_sonar;
 public:
  Csonar(){
    int trigPin = 19;
    int echoPin = 20;
    m_sonar = new NewPing(trigPin, echoPin);
  }
  int GetDistance(){
    unsigned int uS = m_sonar->ping();
    return uS / US_ROUNDTRIP_CM;
  }
};
