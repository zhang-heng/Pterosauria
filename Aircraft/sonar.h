class Csonar
{
 private:
  NewPing * m_sonar;
 public:
  Csonar(int trigPin, int echoPin, int max_distance){
    m_sonar = new NewPing(trigPin, echoPin, max_distance);
  }
  int GetDistance(){
    unsigned int uS = m_sonar->ping();
    return uS / US_ROUNDTRIP_CM;
  }
};
