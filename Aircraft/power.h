class Cpower
{
  public:
    Cpower(int pin)
    {
      m_pin = pin;
    }
    float GetVoltage()
    {
      v = analogRead(m_pin) * 4 * 5 / 1024;
      return map(v, 9, 12.6, 0, 100);
    }
  private:
    float v ;
    int m_pin;
};
