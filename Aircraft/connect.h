class Cconnect
{
public :
  NetStruct netbuffer;
  Cconnect(int cePin, int csnPin)
  {
    //* MISO -> 12
    //* MOSI -> 11
    //* SCK -> 13

    Mirf.cePin = cePin;
    Mirf.csnPin = csnPin;  
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"1aircraft");
    Mirf.channel = 1;
    Mirf.payload = sizeof(NetStruct);
    Mirf.config();
    Mirf.setTADDR((byte *)"1controler");
  }

  void Handle()
  {     
    byte data[Mirf.payload];
    if(!Mirf.isSending() && Mirf.dataReady())
    {
      Mirf.getData(data);
      //Mirf.setTADDR((byte *)"1aircraft");
      Mirf.send(data);
      //Serial.println("Reply sent.");
    }
  }  
};
















