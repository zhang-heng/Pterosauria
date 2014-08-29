//电子罗盘(GY-271 HMC5883L)
class Ccompass
{
private :
  HMC5883L mag;
  int16_t mx, my, mz;

public :
  Ccompass()
  {
    Wire.begin(); 
    Serial.println("HMC5883L: Initializing I2C devices...");
    mag.initialize();
    Serial.print("HMC5883L: Testing device connections...");
    Serial.print(mag.testConnection() ? "successful\n" : "failed\n");
  }

  float GetPoint()
  {
    mag.getHeading(&mx, &my, &mz);
    float heading = atan2(my, mx);
    if(heading < 0)
      heading += 2 * M_PI; 
    return (heading * 180/M_PI);
  }
};




