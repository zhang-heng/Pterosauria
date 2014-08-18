
typedef struct NET_STRUCT
{
  int pitch;//left front&back
  int roll;// left left&right
  int elevation; //right front&back
  int direction;//right left&right
}
NetStruct,*pNetStruct;

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
    Mirf.setRADDR((byte *)"aircraft1");
    Mirf.channel = 10;
    Mirf.payload = sizeof(NetStruct);
    Mirf.config();
    Mirf.setTADDR((byte *)"controler1");
  }

  void Handle()
  {
    if(!Mirf.isSending() && Mirf.dataReady())
    {
      Mirf.getData((byte*) & netbuffer);
      Mirf.send((byte*) & netbuffer);
    }
  }
};





